'use strict';
//Subscribtion for Events
$('#list').on('click', '.deleteMe', function(){
    $(this).parent().remove();
    $('#list').listview('refresh');
    UiWakeList.sendWakeTimeData();
});

$('#list').on('click', '.wakeTimeItem', function(){
    var html = $(this).html();
    var text = $(this).text();
    var timeInfo = text.substr(0,20);
    var alarmInfo;
    var searchResult = html.search(/\<s\>/);
    if (searchResult == -1)
    {
        alarmInfo = "&nbsp;|&nbsp;<b><s>&#x23F0;</s></b>&nbsp;|";
    }
    else
    {
        alarmInfo = "&nbsp;|&nbsp;&#x23F0;&nbsp;|";
    }
    $(this).html(timeInfo + alarmInfo);
    $('#list').listview('refresh');
    UiWakeList.sendWakeTimeData();
});

const wakeTimeTypes = {
    unknown : 0,
    moon : 1,
    sun : 2,
    sunAlarm : 3
};

var WakeListFunc = {

    getWakeUpList : function(wakeDict)
    {
        var wakeUpList = [];
        var keysArr = Array.from(wakeDict.keys());
        keysArr.sort(function(a, b){
            return a - b;
        });

        for (var key of keysArr) {
            const value = wakeDict.get(key);

            const wakeUpListTargetFormat = WakeListFunc.getWakeUpListTargetFormat(key, value);
            wakeUpList.push(wakeUpListTargetFormat);
        }
        return wakeUpList;
    },
    
    getWakeUpListTargetFormat: function(minutes, type)
    {
        var target = minutes << 4;
        target = target | type;
        return target;
    },
    
    createListEntry: function(getUpTime, goToBedTime, alarm)
    {
        var alarmStr;
        if (alarm)
        {
            alarmStr = "&nbsp;|&nbsp;&#x23F0;&nbsp;|";
        }
        else
        {
            alarmStr = "&nbsp;|&nbsp;<s>&#x23F0;</s>&nbsp;|";
        }
        //var text = "|&#x2600;&nbsp;" + getUpTime + "&nbsp;|&nbsp;&#x263E;&nbsp;" + goToBedTime + alarmStr;
        var text = "|&#127774;&nbsp;" + getUpTime + "&nbsp;|&nbsp;&#127772;&nbsp;" + goToBedTime + alarmStr;
        return "<li class='wakeListItem'><a class='wakeTimeItem'>" + text + "</a><a class='deleteMe'></a></li>";
    },

    getAlarmValueFunc : function(html)
    {
        //search returns position of string, if string not found it returns -1
        var pos = html.search(/\<s\>/);
        
        //If it is -1 Alarm is on, alse it is off
        return pos == -1;
    },

    getMinutesPerDay : function (timeStr)
    {
        var hour = parseInt(timeStr.substr(0,2));
        var minutes = parseInt(timeStr.substr(3,2));
        return hour*60 + minutes;
    },

    getWakeUpTimeFunc : function (text)
    {
        var returnValue = text.substr(4,5);
        return returnValue;
    },

    getGoBedTimeFunc : function(text)
    {
        var returnValue = text.substr(15,5);
        return returnValue;
    }
};

var UiWakeList = {
    setWakeTimeList : function(jsonWakeTimeList)
    {
        var timeArr = jsonWakeTimeList.wakeTimes;
        var count = timeArr.length;
    
        for (var i=0; i < count; i++)
        {    
            UiWakeList.addTimeToList(timeArr[i].getUp, timeArr[i].goToBed, timeArr[i].alarm);
        }
        $("#list").listview("refresh");
    },

    addTimeToList : function (getUpTime, goToBedTime, alarm)
    {
        var listEntry = WakeListFunc.createListEntry(getUpTime, goToBedTime, alarm);
        $("#list").append(listEntry);        
    },

    sortList : function(currentList)
    {
        return $(
            $(currentList).toArray().sort(function(a, b){
                var wakeTimeEntryA = $(a).children().eq(0);
                var wakeTimeEntryB = $(b).children().eq(0);
                var aTimeMin = WakeListFunc.getMinutesPerDay(UiWakeList.getWakeUpTime(wakeTimeEntryA)),
                    bTimeMin = WakeListFunc.getMinutesPerDay(UiWakeList.getWakeUpTime(wakeTimeEntryB));
                return aTimeMin - bTimeMin;
            })
        );
    },

    sortWakeList : function()
    {
        var currentList = $(".wakeListItem");        
        var sortedList = this.sortList(currentList);

        $("#list").empty();
        $("#list").append(sortedList);
        $("#list").listview("refresh");
    },

    getAlarmValue : function (wakeUpEntry)
    {
        var html = wakeUpEntry.html();
        return WakeListFunc.getAlarmValueFunc(html);
    },

    getGoBedTime : function (wakeUpEntry)
    {
        var text = wakeUpEntry.text();
        return WakeListFunc.getGoBedTimeFunc(text);
    },

    getWakeUpTime : function (wakeUpEntry)
    {
        var text = wakeUpEntry.text();
        return WakeListFunc.getWakeUpTimeFunc(text);
    },

    getWakeDict : function()
    {
        const num = ($(".wakeTimeItem")).length;

        var wakeDict = new Map();

        for (var i = 0; i < num; i++)
        {
            var wakeEntry = $(".wakeTimeItem").eq(i);
            var time_wakeUp = this.getWakeUpTime(wakeEntry);
            var time_goBed = this.getGoBedTime(wakeEntry);
            var alarm = this.getAlarmValue(wakeEntry);

            var t_wakeUp_minutes = WakeListFunc.getMinutesPerDay(time_wakeUp);
            var t_goBed_minutes = WakeListFunc.getMinutesPerDay(time_goBed);

            if (alarm)
            {
                wakeDict.set(t_wakeUp_minutes, wakeTimeTypes.sunAlarm);
            }
            else
            {
                wakeDict.set(t_wakeUp_minutes, wakeTimeTypes.sun);
            }

            wakeDict.set(t_goBed_minutes, wakeTimeTypes.moon);
        }

        return wakeDict;
    },

    sendWakeTimeData : function ()
    {
        const sortedWakeDict = this.getWakeDict();
        const wakeUpList = WakeListFunc.getWakeUpList(sortedWakeDict);

        var wakeUpData = {
            size : wakeUpList.length,
            dataArr : wakeUpList
        };
        
        const jsonData = JSON.stringify(wakeUpData);

        SleepUinoCom.sendWakeData(jsonData);
    },

    //check if new time entrie has no conflict with available entries in list
    checkTimes : function (wakeUpTime, goBedTime)
    {
        var returnValue = true;
        var wakeUpMin = WakeListFunc.getMinutesPerDay(wakeUpTime);
        var goBedMin = WakeListFunc.getMinutesPerDay(goBedTime);

        //times has to be different
        if ((goBedMin == wakeUpMin) && returnValue)
        {
            returnValue = false;
            alert(LangSupport.getLangString("errorDiffTimeReq"));
        }
        //special case over midnight
        else if ((goBedMin < wakeUpMin) && returnValue)
        {   
            var numOfEntries = $(".wakeTimeItem").length;
            if (numOfEntries > 0)
            {
                var firstEntry = $(".wakeTimeItem").eq(0);
                var lastEntry = $(".wakeTimeItem").eq(numOfEntries-1);
                
                var wakeUpFirst = this.getWakeUpTime(firstEntry);
                var goBedLast = this.getWakeUpTime(lastEntry);

                var wakeUpMinFirst = WakeListFunc.getMinutesPerDay(wakeUpFirst);
                var goBedMinLast = WakeListFunc.getMinutesPerDay(goBedLast);

                if (goBedMin > wakeUpMinFirst || wakeUpMin < goBedMinLast)
                {
                    returnValue = false;
                    alert(LangSupport.getLangString("avoidTimeConflict"));
                }
            }            
        }

        if (returnValue)
        {
            var numOfWakeEntries = $('.wakeTimeItem').length;
            for (var i = 0; i < numOfWakeEntries; i++)
            {
                var wakeEntry = $(".wakeTimeItem").eq(i);
                var wakeUpTimeEntryMin = WakeListFunc.getMinutesPerDay(this.getWakeUpTime(wakeEntry));
                var goBedTimeEntryMin = WakeListFunc.getMinutesPerDay(this.getGoBedTime(wakeEntry));
                
                //check time conflict with available entries
                if ((wakeUpMin >= wakeUpTimeEntryMin && wakeUpMin <= goBedTimeEntryMin) ||
                    (goBedMin >= wakeUpTimeEntryMin && goBedMin <= goBedTimeEntryMin))
                {
                    returnValue = false;
                    alert(LangSupport.getLangString("avoidTimeConflict"));
                    break;
                }                
            }
        }

        
        return returnValue;
    },

    appendToWakeList : function ()
    {
        var t1 = $("#wakeUpTime").val();
        var t2 = $("#goToBedTime").val();
    
        if ($('.wakeTimeItem').length >= 5)
        {
            alert(LangSupport.getLangString("avoidMoreThatFive"));
        }
        else if (t1 != "" && t2 != "")
        {
            if (this.checkTimes(t1, t2))
            {
                this.addTimeToList(t1, t2, false);
                
                this.sortWakeList();
                
                this.sendWakeTimeData();
                //reset times
                $("#wakeUpTime").val("");
                $("#goToBedTime").val("");
            }
        }
        else
        {
            // popup does not work, maybe someone can clarify
            alert(LangSupport.getLangString("enterBothTimes"));
        }
    }
};

var UiFunc = {
    enableTestTab : false,

    enableDisableTestTab : function(){
        var test_switch = $("#test-flip").val();
        console.log("test_switch" + test_switch);
        // this.addTestTab();
    },

    addTestTab : function(){
        if (this.enableTestTab)
        {
            $.getScript("./SleepUinoTests.js", function(){
            TestRunner.addTestTab();
            });
        }
        else
        {
            $("#navbar").navbar();
        }
    },

    addTestSwitch : function(){
        var test_switch = `
        <a data-role="none" class="ui-btn-left">
                <select id="test-flip" name="test-flip" data-role="slider" onchange="UiFunc.enableDisableTestTab()">
                    <option value="off">Test</option>
                    <option value="on">Off</option>
                </select>
            </a>
        `
        if (SleepUinoCom.enableServerCom == false)
        {
            $(test_switch).appendTo("#header");
            $("#test-flip").slider();
        }
    },

    handleChangeDisplayMode : function(){
        var mode = $("#select-display-mode").val();
        SleepUinoCom.sendDisplayMode(mode);
    },

    handleAlarmReplays : function(){
        SleepUinoCom.soundReplay = parseInt($("#alarmReplays").val());
        SleepUinoCom.sendSoundReplay();
    },

    handleFlipAlarm : function(){
        var checked = $( "#flipAlarm" ).prop("checked");
        if (checked)
        {
            $("#alarmReplays").val("1");
            $("#labelAlarmReplays").addClass("ui-disabled");
            $("#alarmReplays").addClass("ui-disabled");
            SleepUinoCom.soundReplay = 0;
            SleepUinoCom.sendSoundReplay();
        }
        else
        {
            $("#labelAlarmReplays").removeClass("ui-disabled");
            $("#alarmReplays").removeClass("ui-disabled");
            SleepUinoCom.soundReplay = parseInt($("#alarmReplays").val());
            SleepUinoCom.sendSoundReplay();
        }
    },

    setSliders : function  (jsonAnswer){
        $("#sliderMoon").val(jsonAnswer.moon).slider('refresh');
        $("#sliderSun").val(jsonAnswer.sun).slider('refresh');
        $("#sliderGain").val(jsonAnswer.gain).slider('refresh');
        $("#select-display-mode").prop("value", jsonAnswer.displayMode).selectmenu("refresh");

        var replays = jsonAnswer.soundReplay;
        
        if (replays == 0)
        {
            $("#flipAlarm").prop("checked", true).flipswitch('refresh');

            $("#alarmReplays").val("1");


            $("#labelAlarmReplays").addClass("ui-disabled");
            $("#alarmReplays").addClass("ui-disabled");

        }
        else
        {
            $("#alarmReplays").val(replays);
            $("#flipAlarm").prop("checked", false).flipswitch('refresh');

            $("#labelAlarmReplays").removeClass("ui-disabled");
            $("#alarmReplays").removeClass("ui-disabled");
        }
        
    },

    checkTimeDiff : function(jsonSleepUinoTime){
        var now = new Date();
        var sleepUinoTime = new Date(jsonSleepUinoTime.year, jsonSleepUinoTime.month, jsonSleepUinoTime.day, jsonSleepUinoTime.hours,
                                 jsonSleepUinoTime.minutes, jsonSleepUinoTime.seconds);
        var text = now.toLocaleString();
        $("#timeBrowser").val(text);            
        
        var text = sleepUinoTime.toLocaleString();
        $("#timeESP").val(text);

        var diff = Math.abs(now.valueOf() - sleepUinoTime.valueOf());
        
        //if diff > 30 sec show sync as black button
        if (diff > 30000)
        {
            $("#syncButton").removeClass("ui-btn-a");
            $("#syncButton").addClass("ui-btn-b");
        }
        else
        {
            $("#syncButton").removeClass("ui-btn-b");
            $("#syncButton").addClass("ui-btn-a");
        }
    },
    
    setDescriptionText : function(tabName){
        var text = "Error: Unknown"
        
        if (tabName == "timeDescription")
        {
            text = LangSupport.getLangString("timeDescription");
        }
        else if (tabName == "wakeTimeDescription")
        {
            text = LangSupport.getLangString("wakeTimeDescription");
        }
        else if (tabName == "ledDescription")
        {
            text = LangSupport.getLangString("ledDescription");
        }
        $("#description-text").html(text);
    }
};