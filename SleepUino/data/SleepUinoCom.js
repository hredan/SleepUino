'use strict';
var SleepUinoCom = {
    //needed for Mockup Communication (in case enableServerCom = false)
    enableServerCom : true,
    isFirstGetTimeRequest : true,
    moonValue : 50,
    sunValue : 60,
    gainValue : 20,
    displayMode : 2,
    soundReplay : 0,
    isNotPlayingSound : true,

    getWakeUpArrayAnswer : "",

    getTime : function (){
        if (this.enableServerCom)
        {
            $.ajax({url: "/getTime", type: "GET", dataType: "json"})
            .done(function(jsonSleepUinoTime){
                UiFunc.checkTimeDiff(jsonSleepUinoTime);
            });
        }
        else
        {
            var now = new Date();
            if (this.isFirstGetTimeRequest)
            {
                now.setHours(now.getHours() + 1);
                this.isFirstGetTimeRequest = false;
            }

            var jsonSleepUinoTimeFake = {"year": now.getFullYear(), "month": now.getMonth(), "day": now.getDate(), 
                "hours": now.getHours(), "minutes": now.getMinutes(), "seconds": now.getSeconds()};
                UiFunc.checkTimeDiff(jsonSleepUinoTimeFake);
        }
    },

    getValues : function(){
        if (this.enableServerCom)
        {
            $.ajax({url: "/getValues", type: "GET", dataType: "json"})
                .done(function(jsonResult){
                    UiFunc.setSliders(jsonResult);;
                });
        }
        else
        {
            var jsonAnswer = {"moon": this.moonValue, "sun": this.sunValue, "gain": this.gainValue, 
                                "displayMode": this.displayMode, "soundReplay": 3}
            UiFunc.setSliders(jsonAnswer);
            console.log("FakeCom: get->" + JSON.stringify(jsonAnswer));
        }
    },

    syncTime : function () {
        var now = new Date();
        var jsonTimeObj = {"year": now.getFullYear(), "month": now.getMonth(), "day": now.getDate(), 
            "hours": now.getHours(), "minutes": now.getMinutes(), "seconds": now.getSeconds()};
        
        if (this.enableServerCom)
        {
            $.ajax({url: "/syncTime", type: "POST", dataType: "json", data: jsonTimeObj})
            .done(function(jsonAnswer){
                //update Time and check diff again
                if(jsonAnswer.success)
                {
                    $("#syncButton").blur();
                    SleepUinoCom.getTime();
                }                
            });
        }
        else
        {
            $("#syncButton").blur();
            this.getTime();
        }
       
    },

    Sleep: function (milliseconds) {
        return new Promise(resolve => setTimeout(resolve, milliseconds));
    },

    playFakeSound : async function(){
        if (this.isNotPlayingSound)
        {
            this.isNotPlayingSound = false;
            var audio = new Audio('Kikeriki.wav');
            audio.volume = this.gainValue/100.0;
            audio.play();
            await this.Sleep(3000); 
            $("#playButton").blur();
            console.log("Play Sound");
            this.isNotPlayingSound = true; 
        }
    },

    playSound : function (){
        if (this.enableServerCom)
        {
            $.ajax({url: "/playSound", type: "GET", dataType: "json", timeout: 10000})
            .done(function(jsonResult){
                if(jsonResult.success)
                {
                    $("#playButton").blur();
                }
                else
                {
                    console.log("Error: playSound");
                }
            });
        }
        else
        {
           this.playFakeSound();
        }
    },

    setAlarmVolume : function (volumeValue)
    {
        if (this.enableServerCom)
        {
            var jsonTimeObj = {"gain": volumeValue};
            $.ajax({url: "/setGain", type: "POST", dataType: "json", data: jsonTimeObj})
            .success(function(result){
                console.log(result);
            });
        }
        else
        {
            this.gainValue = volumeValue;
        }
    },

    sendDataSun : function (brightness)
    {
       if (this.enableServerCom)
       {
            $.ajax({url: "/setLEDSun", type: "POST", dataType: "text", data: brightness})
            .success(function(result){
                console.log(result);
            });
       }
       else
       {
            this.sunValue = brightness;
       }
    },

    sendWakeData : function (jsonData){
        if (this.enableServerCom)
        {
            $.ajax({url: "/setWakeData", type: "POST", dataType: "json", data: jsonData})
            .success(function(result){
                console.log(result);
            });
        }
        else
        {
            console.log("FakeCom: send Data -> " + jsonData);
        }
    },
    
    sendDataMoon : function (brightness){
        
        if (this.enableServerCom)
        {
            $.ajax({url: "/setLEDMoon", type: "POST", dataType: "text", data: brightness})
            .success(function(result){
                console.log(result);
            });
        }
        else
        {
            this.moonValue = brightness;
        }
    },

    sendSoundReplay : function (){
        if (this.enableServerCom)
        {
            $.ajax({url: "/setSoundReplay", type: "POST", dataType: "text", data: (SleepUinoCom.soundReplay).toString()})
            .success(function(result){
                console.log(result);
            });
        }
        else
        {
            console.log("FakeCom: send soundReplay: " + this.soundReplay);
        }
    },

    sendDisplayMode : function (displayMode){
        
        if (this.enableServerCom)
        {
            $.ajax({url: "/setDisplayMode", type: "POST", dataType: "text", data: displayMode})
            .success(function(result){
                console.log(result);
            });
        }
        else
        {
            this.displayMode = displayMode;
            console.log("FakeCom: send displayMode: " + displayMode);
        }
    },

    getWakeUpArray : function ()
    {
        
        if (this.enableServerCom)
        {
            $.ajax({url: "/getWakeTimeData", type: "GET", dataType: "json"})
                .done(function(jsonResult){
                    console.log("getWakeTimeData: " + JSON.stringify(jsonResult));
                    UiWakeList.setWakeTimeList(jsonResult);
                });
        }
        else
        {
            var jsonWakeTimesAnswer
            jsonWakeTimesAnswer = {"wakeTimes":[{"getUp":"01:00", "goToBed": "02:00", "alarm": true}, {"getUp":"03:00", "goToBed": "04:00", "alarm": true}, {"getUp":"06:00", "goToBed": "09:00", "alarm": true}, {"getUp":"12:00", "goToBed": "15:00", "alarm": false}]};
            UiWakeList.setWakeTimeList(jsonWakeTimesAnswer);
        }
    }
};