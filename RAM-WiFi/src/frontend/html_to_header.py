import re
with (open('frontend.h', 'w') as output, 
      open('index.html', 'r') as html,
      open('index.css', 'r') as css,
      open('index.js', 'r') as js):
    frontend = re.sub(
        r'\s{2,}|\n', '', #remove whitespace of length > 2 or \n's
        re.sub(
            r'//.*|/\*(\S|\s)*\*/', '', #delete all comments
            html.read()
                .replace('/*css*/', css.read())
                .replace('/*js*/', js.read())
                .replace('"', '\\"')
        )
    )
    output.write(f'const char* FRONTEND = "{frontend}";')