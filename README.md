# Smart_Irrigation_Controller

* Note: Must add #define __TI_TIME_USES_64 1 to time.h to make epoch correct.
* Current functionality:
    * Correctly fetches weather data from [OpenWeatherMap](https://openweathermap.org/api)
    * Correctly sychronizes time with [WorldTimeAPI](https://worldtimeapi.org/timezones)
    * All GUI pages finished except for the scheduling page
    * Buttons inside GUI pages are functional
    * Homepage displays weather data with corresponding icons
* Tasks left:
    * Program navigaiton bar buttons to switch between GUI pages
    * Configure GPIO (or use a shift register) to control irrigation switch relays
    * Store user configuration in flash

## Finished GUI Pages
### Home
* Displays the current time
* Displays the current weather and the weather forecast in the next few days
* ![home1](https://github.com/shida-yang/Smart_Irrigation_Controller/blob/master/GUI_Demos/home1.jpg)
* ![home2](https://github.com/shida-yang/Smart_Irrigation_Controller/blob/master/GUI_Demos/home2.jpg)
* ![home3](https://github.com/shida-yang/Smart_Irrigation_Controller/blob/master/GUI_Demos/home3.jpg)
### Setting
* Controls some basic settings like time synchronization, brightness, and screen timeout
* When AUTO is selected, the time/brightness will be controlled by program, and user cannot change the setting
* ![setting](https://github.com/shida-yang/Smart_Irrigation_Controller/blob/master/GUI_Demos/setting.gif)
### Plan
* Set different plans for each irrigation session
* Each zoom can be watered up to 99 minutes
* The user can choose to active/deactive any zoom for each plan
* The user can only change irrigation time for active zooms
* ![plan](https://github.com/shida-yang/Smart_Irrigation_Controller/blob/master/GUI_Demos/planning.gif)
### Watering Status
* This page shows the watering status when an irrigation session is going on
* The zooms will be watered automatically in order
* User can manually jump to any of the zooms
* ![watering](https://github.com/shida-yang/Smart_Irrigation_Controller/blob/master/GUI_Demos/watering.gif)
### Scheduling
* This page controls which days of a week the user wants to irrigate (the real irrigation decision will incorporate this setting and the weather data)
* The irrigation time for each zoom will be controlled by the selected plan
* To be implemented
