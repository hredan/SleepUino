'use strict';
var LangSupport = {
    lang : "de",
    langResources : undefined,

    createLangResources : function(){
        var en = new Array();
        var de = new Array();
        var ru = new Array();

        //Footer translations
        de['footerHeadline'] = "Beschreibung";
        en['footerHeadline'] = "Description";
        ru['footerHeadline'] = "Описание";

        de['timeDescription'] = "Hier werden die aktuellen Zeiten vom SleepUINO und ihrem Gerät angezeigt, wenn die Zeiten zu stark abweichen, können sie mit der Taste 'Zeit synchronisieren' die Zeit abgleichen.";
        en['timeDescription'] = "If the time is different between Browser and SpeedUINO then the time can be synchronized by button 'Synchronize time'.";
        ru['timeDescription'] = "Если время в браузере и SpeedUINO отличается, то время можно синхронизировать с помощью кнопки 'Синхронизировать время'."

        de['ledDescription'] =  "Mit den beiden oberen Schiebern, läßt sich die Helligkeit der LEDs (Mond und Sonne) verändern. " +
                                "Im unteren Bereich, kann man den Alarm abspielen und die Lautstärke verändern.";
        en['ledDescription'] =  "With the two upper sliders, the brightness of the LEDs (moon and sun) can be changed. " + 
                                "In the lower area, you can play the alarm and change the volume.";
        ru['ledDescription'] =  "С помощью двух верхних ползунков можно изменять яркость светодиодов (луна и солнце). " + 
                                "В нижней части вы можете включить будильник и изменить громкость.";

        de['wakeTimeDescription'] = "In 'Eingabe der Weck- und Schlafenszeit' kann ein Wach-Zeitbereich eingegeben werden und mit der Hinzufügen Tast zur Liste hinzugefügt werden. " + 
                                    "&#x1F31E; steht für die Weckzeit und &#x1F31C; für die Zeit um ins Bett zu gehen. " +
                                    "Mit der X Taste können Einträge aus der Liste gelöscht werden, drückt man auf den Eintrag selber kann man den Alarm ein- und ausschalten (&#x23F0;/<s>&#x23F0;</s>).";
        en['wakeTimeDescription'] = "In 'Enter the wake-up and bedtime', a wake-up time range can be entered and added to the list with the Add button. " +
                                    "&#x1F31E; means the wake-up time and &#x1F31C; means the time to go to bed. " +
                                    "Entries can be deleted from the list with the X button, if you press the entry itself you can switch the alarm on/off (&#x23F0;/<s>&#x23F0;</s>).";
        ru['wakeTimeDescription'] = "В поле 'Введите время пробуждения и отхода ко сну' можно ввести диапазон времени пробуждения и добавить его в список с помощью кнопки 'Добавить'. " +
                                    "&#x1F31E; означает время пробуждения и &#x1F31C; означает время ложиться спать. " +
                                    "Записи могут быть удалены из списка с помощью клавиши X, если вы нажмете саму запись, вы можете включить/выключить будильник (&#x23F0;/<s>&#x23F0;</s>).";

        //name of tabs
        de['tabTime'] = "Zeit";
        en['tabTime'] = "Time";
        ru['tabTime'] = "Время";

        de['tabLED'] = "LED/Alarm";
        en['tabLED'] = "LED/Alarm";
        ru['tabLED'] = "LED/Тревога";

        de['tabWakeTime'] = "Weckzeiten";
        en['tabWakeTime'] = "Alarm times";
        ru['tabWakeTime'] = "Время будильника";

        //Time Page
        de['headerTimeSetup'] = "Zeit Einstellung";
        en['headerTimeSetup'] = "Time setting";
        ru['headerTimeSetup'] = "Настройки времени";

        de['labelTimeBrowser'] = "Browser Zeit:";
        en['labelTimeBrowser'] = "browser time:";
        ru['labelTimeBrowser'] = "время браузера:";

        de['labelTimeESP'] = "SleepUINO Zeit:";
        en['labelTimeESP'] = "SleepUINO time:";
        ru['labelTimeESP'] = "SleepUINO время:";

        de['syncButton'] = "Zeit synchronisieren";
        en['syncButton'] = "Synchronize time";
        ru['syncButton'] = "Синхронизировать время";

        //Alarm times page
        de['headerLED'] = "LED Einstellungen";
        en['headerLED'] = "LED Settings";
        ru['headerLED'] = "Настройки светодиода";

        de['labelSliderMoon'] = "Helligkeit LED Mond:";
        en['labelSliderMoon'] = "Brightness LED moon:";
        ru['labelSliderMoon'] = "Яркость светодиодной луны:";
        
        de['labelSliderSun'] = "Helligkeit LED Sonne:";
        en['labelSliderSun'] = "Brightness LED sun:";
        ru['labelSliderSun'] = "Яркость светодиода солнце:";

        de['headerAlarm'] = "Alarm Einstellungen";
        en['headerAlarm'] = "Alarm settings";
        ru['headerAlarm'] = "Настройки будильника";

        de['headerDisplay'] = "Anzeige Einstellungen";
        en['headerDisplay'] = "Display Settings";
        ru['headerDisplay'] = "Настройки отображения";

        de["display-on"] ="Anzeige-An";
        en["display-on"] ="Display-On";
        ru["display-on"] ="Дисплей включен";

        de["display-off"] = "Anzeige-Aus";
        en["display-off"] = "Display-Off";
        ru["display-off"] = "Дисплей выключен";

        de["display-on-day"] ="Anzeige-An zur Wachzeit";
        en["display-on-day"] ="Display-On at waking time";
        ru["display-on-day"] ="Включение дисплея во время бодрствования";

        de["display-on-night"] = "Anzeige-An zur Schlafenszeit";
        en["display-on-night"] = "Display-On at bedtime";
        ru["display-on-night"] = "Включение дисплея перед сном";

        de['playButton'] = "Sound abspielen";
        en['playButton'] = "Play sound";
        ru['playButton'] = "Воспроизвести звук";

        de['labelSliderGain'] = "Sound Lautstärke:";
        en['labelSliderGain'] = "Sound Volume:";
        ru['labelSliderGain'] = "громкости:";

        de['labelFlipAlarm'] = "Alarm abspielen bis die Taste gedrückt wird:";
        en['labelFlipAlarm'] = "Play the alarm until the button is pressed:";
        ru['labelFlipAlarm'] = "Включите будильник, пока не будет нажата кнопка:";

        de['optOnAlarm'] = "An";
        en['optOnAlarm'] = "On";
        ru['optOnAlarm'] = "Вкл";

        de['optOffAlarm'] = "Aus";
        en['optOffAlarm'] = "Off";
        ru['optOffAlarm'] = "Выкл";

        de['labelAlarmReplays'] = "Anzahl der Wiederholungen des Sounds:";
        en['labelAlarmReplays'] = "Number of times to repeat the sound:";
        ru['labelAlarmReplays'] = "Количество раз, чтобы повторить звук:";

        //WakeTimeList
        de['headerSetWakeTimes'] = "Eingabe der Weck- und Schlafenszeit:";
        en['headerSetWakeTimes'] = "Enter the wake up time and bedtime:";
        ru['headerSetWakeTimes'] = "Введите время пробуждения и перед сном:";

        de['labelWakeUpTime'] = "&#127774; Weckzeit:";
        en['labelWakeUpTime'] = "&#127774; Wake up time:";
        ru['labelWakeUpTime'] = "&#127774; Время пробуждения";

        de['labelGoBedTime'] = "&#127772; Schlafenszeit:";
        en['labelGoBedTime'] = "&#127772; Bedtime:";
        ru['labelGoBedTime'] = "&#127772; Перед сном:";

        de['addTimeBtn'] = "Zeitbereich der Liste unten hinzufügen";
        en['addTimeBtn'] = "Add the time range to the list below";
        ru['addTimeBtn'] = "Добавьте временной диапазон в список ниже";
        
        de['headerTimeList'] = "Weckzeiten Liste";
        en['headerTimeList'] = "Alarm times list";
        ru['headerTimeList'] = "Список времени будильника";
        //error messages
        de['avoidMoreThatFive'] = "Mehr als 5 Zeiteinträge sind nicht möglich.";
        en['avoidMoreThatFive'] = "More than 5 time entries are not possible.";
        ru['avoidMoreThatFive'] = "Запись более 5 раз невозможна.";

        de['enterBothTimes'] = "Bitte geben Sie beide Zeiten ein, bevor sie die Zeiten zur Liste hinzufügen."
        en['enterBothTimes'] = "Please enter both times before you can add they to the list."
        ru['enterBothTimes'] = "Пожалуйста, введите оба раза, прежде чем добавлять время в список."

        de["errorDiffTimeReq"] = "Die Zeiten müssen unterschiedlich sein.";
        en["errorDiffTimeReq"] = "The times must be different."; 
        ru["errorDiffTimeReq"] = "Времена должны быть другие.";

        de["avoidTimeConflict"] = "Die Zeiten überschneiden sich mit anderen Zeitbereichen in der List."
        en["avoidTimeConflict"] = "The times overlap with other time ranges in the list."
        ru["avoidTimeConflict"] = "Время перекрывается с другими временными диапазонами в списке."


        //create one Array with all language information
        var resources = new Array();
        resources['de'] = de;
        resources['en'] = en;
        resources['ru'] = ru;

        this.langResources = resources;
    },
    
    changeLang : function()
    {
        var newLang = $("#select-lang").val();
        this.setLanguage(newLang);

        //find active tab, and set description text
        if ($("#tabTime").hasClass("ui-btn-active"))
        {
            UiFunc.setDescriptionText('timeDescription');
        }
        else if ($("#tabLED").hasClass("ui-btn-active"))
        {
            UiFunc.setDescriptionText('ledDescription');
        }
        else if ($("#tabWakeTime").hasClass("ui-btn-active"))
        {
            UiFunc.setDescriptionText('wakeTimeDescription');
        }

        //set footerHeadline
        this.setLangOfElement("footerHeadline");

        //set name of tabs
        this.setLangOfElement("tabTime");
        this.setLangOfElement("tabLED");
        this.setLangOfElement("tabWakeTime");

        //Time Page
        this.setLangOfElement("headerTimeSetup");
        this.setLangOfElement("labelTimeBrowser");
        this.setLangOfElement("labelTimeESP");
        this.setLangOfElement("syncButton");


        //LED Alarm page
        this.setLangOfElement("headerLED");
        this.setLangOfElement("labelSliderMoon");
        this.setLangOfElement("labelSliderSun");
        this.setLangOfElement("headerAlarm");
        this.setLangOfElement("playButton");
        this.setLangOfElement("labelSliderGain");
        this.setLangOfElement("labelFlipAlarm");
        this.setLangOfElement("headerDisplay");
        
        this.setLangOfElement("display-on");
        this.setLangOfElement("display-off");
        this.setLangOfElement("display-on-day");
        this.setLangOfElement("display-on-night");
        $("#select-display-mode").val($("#select-display-mode").val());
        $("#select-display-mode").selectmenu("refresh");

        var onTextStr = this.getLangString("optOnAlarm");
        var offTextStr =  this.getLangString("optOffAlarm");
        $("#flipAlarm").flipswitch("option", "onText", onTextStr);
        $("#flipAlarm").flipswitch("option", "offText", offTextStr);

        this.setLangOfElement("labelAlarmReplays");

        //WakeTimeList
        this.setLangOfElement("headerSetWakeTimes");
        $("#addTimeBtn").val(this.getLangString("addTimeBtn"));
        $("#addTimeBtn").button('refresh');
        this.setLangOfElement("headerTimeList");
        this.setLangOfElement("labelWakeUpTime");
        this.setLangOfElement("labelGoBedTime");
    },

    setLangOfElement: function(id)
    {
        $("#"+id).html(this.getLangString(id));
    },

    setLanguage : function(countryCode)
    {
        //If you add a new language, add the language also to the selection tab!
        if (/^de\b/.test(countryCode) || /^at\b/.test(countryCode) || /^ch\b/.test(countryCode))
        {
            this.lang = "de";
        }
        else if (/^ru\b/.test(countryCode))
        {
            this.lang = "ru";
        }
        else
        {
            this.lang = "en";
        }

        $("#select-lang").val(this.lang);
    },

    getLangString : function(requiredElement)
    {
        var langString = this.langResources[this.lang][requiredElement];
        if (langString == undefined)
        {
            langString = "Unknow";
        }
        return langString;
    }
};