const panel = document.getElementById(`panel`),
      responseDisplay = document.getElementById(`response`),
      accelerator = document.getElementById(`accelerator`),
      reverseToggle = document.getElementById(`reverse`);

const Command = {
    MOVE_FORWARD: 0,
    MOVE_BACKWARD: 1,
    TURN_LEFT: 2,
    TURN_RIGHT: 3,
    LEFT_SWEEP: 4,
    RIGHT_SWEEP: 5,
    SWEEP_ROUTINE: 6,
    RAM: 7,
    STOP: 8,
};

let history = { //map for each sensor's last value to "debounce" amongst other settings
    JoySticks: [],
    Dials: [],
};

const bufferDifferenceThreshold = 5,
      adcResolution = 255,
      _ = undefined;

(() => {
    acceleratorSetup();
    reverseSetup();
    joystickSetup();
    dialSetup();
    
    document.getElementById(`battering-ram`).addEventListener(`click`, () => fetch(`?cmd=${Command.RAM}`));
    document.getElementById(`sweep-toggle`).addEventListener(`click`, () => fetch(`?cmd=${Command.SWEEP_ROUTINE}`));
    document.getElementById(`left-sweep`).addEventListener(`click`, () => fetch(`?cmd=${Command.LEFT_SWEEP}`));
    document.getElementById(`right-sweep`).addEventListener(`click`, () => fetch(`?cmd=${Command.RIGHT_SWEEP}`));
})();

function percentDifference(newValue, oldValue) { return Math.abs(newValue - oldValue)/oldValue * 100; }

function getTouchID(e, target) {
    for(const i in e.touches)
        if(e.touches[i].target == target || target.contains(e.touches[i].target))
            return i;
    return -1;
}

function issueCommand(commandID, value) {
    fetch(!value ? `?cmd=${commandID}` : `?cmd=${commandID}-${value}`);
} 

function stepOnBreak(element) {
    element.classList.remove(`moving`);
    issueCommand(Command.STOP);
}

function acceleratorSetup() {
    accelerator.addEventListener(`touchstart`, () => {
        accelerator.classList.add(`moving`); //acceleration can be set using a recursive timeout
        issueCommand(Command.MOVE_FORWARD, adcResolution);
        responseDisplay.innerHTML = `?cmd${Command.MOVE_FORWARD}-${adcResolution}`;
    });
    accelerator.addEventListener(`touchend`, () => stepOnBreak(accelerator));
}

function reverseSetup() {
    const reverse = document.getElementById(`reverse`);
    reverse.addEventListener(`touchstart`, () => {
        reverse.classList.add(`moving`); //acceleration can be set using a recursive timeout
        issueCommand(Command.MOVE_BACKWARD, adcResolution);
        responseDisplay.innerHTML = `?cmd${Command.MOVE_BACKWARD}-${adcResolution}`;
    });
    reverse.addEventListener(`touchend`, () => stepOnBreak(reverse));
}

function joystickSetup() { //unused
    for(const stick of document.getElementsByClassName(`stick`)) {
        history.JoySticks.push({x: 0, y: 0});
        const id = history.JoySticks.length - 1; //essentially the index of the joystick 
        stick.addEventListener(`touchmove`, e => {
            if(!e.touches) return;
            e.preventDefault();
            const touchID = getTouchID(e, stick),
                  center = (stick.clientWidth)/2, //its a square so these coordinates will always be equal for x,y
                  x = e.touches[touchID].clientX - panel.offsetLeft - stick.parentElement.offsetLeft - center,
                  y = e.touches[touchID].clientY - panel.offsetTop - stick.parentElement.offsetTop - center;
            if(x >= -0.5*center && x <= 1.5*center) {
                stick.style.left = `${x}px`;
                const PWM = Math.floor((x+0.5*center)/stick.clientWidth*adcResolution);
                if(percentDifference(PWM, history.JoySticks.x) > bufferDifferenceThreshold) {
                    history.JoySticks[id].x = PWM;
                    console.log(`Joystick ${id} - fetched`);
                    //fetch
                }
            }
            if(yEnabled && y >= -0.5*center && y <= 1.5*center) {
                stick.style.top = `${y}px`;
                const PWM = Math.floor((y+0.5*center)/stick.clientWidth*adcResolution);
                if(percentDifference(PWM, history.JoySticks.y) > bufferDifferenceThreshold) {
                    history.JoySticks[id].y = PWM;
                    console.log(`Joystick ${id} - fetched`);
                    //fetch
                }
            }
        });
        stick.addEventListener(`touchend`, () => {
            stick.style.left = stick.style.top = ``;
            history.JoySticks.x = history.JoySticks.y = adcResolution/2; //idk if we want to keep a reset
        });
    }
}

function dialSetup() {
    for(const dial of document.getElementsByClassName("dial")) {
        history.Dials.push({ direction: ``, angle: 0 });
        const id = history.Dials.length - 1;
        dial.addEventListener(`touchmove`, e => {
            e.preventDefault();
            const touchID = getTouchID(e, dial),
                  {left, top, width, height} = dial.getBoundingClientRect(),
                  centerDistanceX = e.touches[touchID].clientX - (left + width/2),
                  centerDistanceY = e.touches[touchID].clientY - (top + height/2),
                  upperBound = 180;
                  //+90 bc upwards is now 0 therefore 0-180 is right, 360-180 is left
            let angle = Math.round(Math.atan2(centerDistanceY, centerDistanceX) * 180/Math.PI) + 90;
            if(angle <= 0) 
                angle += 360;
            if(angle < upperBound && (history.Dials[id].direction == Command.TURN_RIGHT || !history.Dials[id].direction)) {
                dial.style.transform = `rotate(${angle}deg)`;
                history.Dials[id].direction = Command.TURN_RIGHT;
                if(angle%5 == 0 && angle != history.Dials[id].angle) {
                    history.Dials[id].angle = angle;
                    issueCommand(Command.TURN_RIGHT, Math.round(angle/upperBound*adcResolution));
                    responseDisplay.innerHTML = `?cmd=${Command.TURN_RIGHT}-${Math.round(angle/upperBound*adcResolution)}`;
                } else
                    responseDisplay.innerHTML = `${history.Dials[id].angle} ${angle}`;
            } else if(angle > upperBound && (history.Dials[id].direction == Command.TURN_LEFT ||  !history.Dials[id].direction)) {
                dial.style.transform = `rotate(${angle}deg)`;
                history.Dials[id].direction = Command.TURN_LEFT;
                if(angle%5 == 0 && angle != history.Dials[id].angle) {
                    history.Dials[id].angle = angle;
                    issueCommand(Command.TURN_LEFT, Math.round((360-angle)/upperBound*adcResolution));
                    responseDisplay.innerHTML = `?cmd=${Command.TURN_LEFT}-${Math.round((360-angle)/upperBound*adcResolution)}`;
                } else
                    responseDisplay.innerHTML = `${history.Dials[id].angle} ${angle}`;
            }
            if(percentDifference(angle, 360) < bufferDifferenceThreshold)
                history.Dials[id].direction = _;
        });
        dial.addEventListener(`touchend`, () => {
            dial.style.removeProperty(`transform`); 
            history.Dials[id].direction = ``;
            history.Dials[id].angle = 360;
            issueCommand(Command.STOP);
            //send a request to stop turning
        });
    }
}