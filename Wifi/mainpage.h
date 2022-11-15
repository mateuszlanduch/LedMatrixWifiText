/*

This file contains HTML code for app of WiFi Message Board

*/

const String page PROGMEM = "<head>"
            " <meta name='viewport' content='width=device-width, initial-scale=1.0'>"
            " <style>"
            " body{"
            " background-color:white;"
            " font-family: 'Roboto', sans-serif;" 
            "}"
            "</style>"
            " </head>"
            " <body>"
            " <h1>Matrix LED 8x32</h1>"
            " <p>WRITE TEXT & CLICK ENTER</p>"
            " <form action='/dataHandler' method='POST'>\r\n"
            " <label for='message'>Message:</label> <input type='text' name='message' id='message'>""</p>\r\n" 
            " <input width='100' height='40' type='submit' id='send' value=' ENTER ' />\r\n "
            " </form> \r\n"
            "</body>";
