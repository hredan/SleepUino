![SleepUino DIY Alarm-Clock with webinterface](./Logo_en.png)
# SleepUino Web Interface
With the help of [github pages](https://pages.github.com/), it is possible to show the SleepUino Web Interface directly in the browser. Try it with the following link:

[show SleepUino web interface in browser](https://hredan.github.io/SleepUino/SleepUino/data/index.html){:target="\_blank"}

## SleepUino Dummy Interface
For an easier development of the jQuery Mobile Web Interface I have used a dummy interface. It is enabled by default. If you want to use the Interface on the ESP, the data directory must be contain the file ./IsSleepUino, in this case the dummy interface will be disabled and the HTTP Request will be sent to the ESP.
