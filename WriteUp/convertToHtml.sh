#!/bin/bash

preamble_html=$(cat << PREAMBLE_HTML
<!DOCTYPE html>

<html>
    <head>
        <title>Edge Match: The Pattern Puzzle Solver</title>
        <meta charset="UTF-8">
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
      .codeblock {
        font-size: 130%;
        spacing-between: 20;
        border: 1px solid gray;
        box-shadow: 0.5px 0.5px 10px gray;
        width: 50%;
        margin: auto;
        margin-top: 30px;
        margin-bottom: 20px;
        padding: 10px;
        width: 90%;
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

scp write-up.html cgregg@myth.stanford.edu:WWW/EdgeMatch/edge-match-draft.html

open -a "Google Chrome" http://web.stanford.edu/~cgregg/EdgeMatch/edge-match-draft.html


