#!/bin/bash

preamble_html=$(cat << PREAMBLE_HTML
<!DOCTYPE html>
<html>
    <head>
        <title>Edge Match: The Pattern Puzzle Solver</title>
        <style>
        .content {
            max-width: 80%;
            margin: auto;
        }
        html, body {
            color: #000;
            font-family: 'arial';
        }
        table, th, td {
            border: 1px solid black;
            border-collapse: collapse;
        }
        code {
            font-size: 130%;
        }
        </style>
    </head>
    <body>
        <div class="content">
PREAMBLE_HTML
)

finale_html=$(cat << FINALE_HTML
        </div>
    </body>
</html>
FINALE_HTML
)

write_up_html=$(pandoc -f markdown -t html write-up.mdown)

echo "$preamble_html" > write-up.html
echo "$write_up_html" >> write-up.html
echo "$finale_html" >> write-up.html

open write-up.html -a "Google Chrome"


