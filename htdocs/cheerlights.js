// cheerlights.js - Copyright 2021 - Tarim
//
//  The MIT License (MIT)
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the “Software”), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following
// conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

// Cheerlights.URL     URL which provides colour as text string
// Cheerlights.period  Interval between polls in milliseconds
// Cheerlights.start   Method to start polling
// Cheerlights.stop    Method to stop polling
// Cheerlights.poll    Internal method to do the work
//
// updates class cheerlightsColor with a background colour
// updates class cheerlightsUpdate with name of colour and last update time

Cheerlights = {
    URL: "http://api.thingspeak.com/channels/1417/field/1/last.txt",
    period: 60000,

    poll: function() {
        fetch (Cheerlights.URL).
        then ((response) => response.text()).
        then ((data) => {
            document.querySelectorAll ('.cheerlightsColor').forEach (
                (element) => {
                    element.style.backgroundColor = data;
                }
            )

            document.querySelectorAll ('.cheerlightsUpdate').forEach (
                (element) => {
                    element.innerHTML = `${data} ${new Date()}`;
                }
            )
        });
    },

    stop: function() {
        if (Cheerlights.timer) {
            clearTimeout (Cheerlights.timer);
            Cheerlights.timer = null;
        }
    },

    start: function() {
        Cheerlights.stop();
        Cheerlights.timer = setInterval (Cheerlights.poll, Cheerlights.period);
        Cheerlights.poll();
    },
};

Cheerlights.start();
