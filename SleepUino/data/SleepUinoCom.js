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
            var audio = new Audio('default_AlarmSound.wav');
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

    stopSound : function (){
        if (this.enableServerCom)
        {
            $.ajax({url: "/stopSound", type: "GET", dataType: "json", timeout: 10000})
            .done(function(jsonResult){
                if (!jsonResult.success)
                {
                    console.log("Error: stopSound");
                }
            });
        }
        else
        {
            this.isNotPlayingSound = true;
            console.log("Stop Sound");
        }
    },

    resetAlarmSound : function (){
        var statusElement = $("#alarmSoundUploadStatus");

        if (this.enableServerCom)
        {
            $.ajax({url: "/resetAlarmSound", type: "GET", dataType: "json", timeout: 10000})
            .done(function(jsonResult){
                if (jsonResult.success)
                {
                    statusElement.text(LangSupport.getLangString("alarmSoundResetSuccess"));
                    $("#popupResetAlarmSound").popup("close");
                }
                else
                {
                    statusElement.text(LangSupport.getLangString("alarmSoundResetFailed"));
                }
            })
            .fail(function(){
                statusElement.text(LangSupport.getLangString("alarmSoundResetFailed"));
            });
        }
        else
        {
            statusElement.text(LangSupport.getLangString("alarmSoundResetDummy"));
        }
    },

    uploadAlarmSound : function (){
        var fileInput = $("#alarmSoundFile")[0];
        var statusElement = $("#alarmSoundUploadStatus");
        var progressElement = $("#alarmSoundUploadProgress");

        progressElement.val(0);

        if (fileInput.files.length === 0)
        {
            statusElement.text(LangSupport.getLangString("alarmSoundSelectFile"));
            return;
        }

        var file = fileInput.files[0];
        var formData = new FormData();
        formData.append("file", file, file.name);

        var startUpload = function() {
            statusElement.text(LangSupport.getLangString("alarmSoundUploading"));

            $.ajax({
                url: "/uploadAlarmSound",
                type: "POST",
                data: formData,
                processData: false,
                contentType: false,
                dataType: "json",
                xhr: function() {
                    var xhr = $.ajaxSettings.xhr();
                    if (xhr.upload)
                    {
                        xhr.upload.addEventListener("progress", function(event) {
                            if (event.lengthComputable)
                            {
                                var percent = Math.round((event.loaded / event.total) * 100);
                                progressElement.val(percent);
                            }
                        }, false);
                    }
                    return xhr;
                }
            })
            .done(function(jsonResult){
                if (jsonResult.success)
                {
                    progressElement.val(100);
                    statusElement.text(LangSupport.getLangString("alarmSoundUploadSuccess"));
                    $("#alarmSoundFile").val("");
                    $("#popupUploadAlarmSound").popup("close");
                    $("#alarmSoundUploadProgress").val(0);
                }
                else
                {
                    progressElement.val(0);
                    statusElement.text(LangSupport.getLangString("alarmSoundUploadFailed"));
                }
            })
            .fail(function(){
                progressElement.val(0);
                statusElement.text(LangSupport.getLangString("alarmSoundUploadFailed"));
            });
        };

        if (this.enableServerCom)
        {
            $.ajax({
                url: "/getMaxSoundSize",
                type: "GET",
                dataType: "json",
                timeout: 10000
            })
            .done(function(jsonResult){
                var maxSoundSize = parseInt(jsonResult.maxSoundSize, 10);

                if (isNaN(maxSoundSize) || maxSoundSize <= 0)
                {
                    progressElement.val(0);
                    statusElement.text(LangSupport.getLangString("alarmSoundUploadFailed"));
                    return;
                }

                if (file.size > maxSoundSize)
                {
                    progressElement.val(0);
                    statusElement.text(
                        LangSupport.getLangString("alarmSoundFileTooLarge") +
                        " (" + file.size + " > " + maxSoundSize + " Bytes)"
                    );
                    return;
                }

                startUpload();
            })
            .fail(function(){
                progressElement.val(0);
                statusElement.text(LangSupport.getLangString("alarmSoundUploadFailed"));
            });
        }
        else
        {
            progressElement.val(100);
            statusElement.text(LangSupport.getLangString("alarmSoundUploadDummy"));
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
            jsonWakeTimesAnswer = {"wakeTimes":[{"getUp":"06:00", "goToBed": "13:00", "alarm": true}, {"getUp":"14:30", "goToBed": "19:00", "alarm": false}]};
            UiWakeList.setWakeTimeList(jsonWakeTimesAnswer);
        }
    },

    initWifiPasswordValidation : function ()
    {
        var passwordInput = $("#wifiPasswordInput");
        var passwordConfirmInput = $("#wifiPasswordConfirmInput");
        var saveButton = $("#wifiPasswordButton");
        var statusElement = $("#wifiPasswordStatus");

        var validate = function() {
            var password = passwordInput.val();
            var passwordConfirm = passwordConfirmInput.val();

            if (password.length > 63)
            {
                saveButton.addClass("ui-state-disabled");
                saveButton.prop("disabled", true);
                statusElement.text(LangSupport.getLangString("wifiPasswordTooLong") || "Password too long (max 63 characters)");
                statusElement.css("color", "#F44336");
                return false;
            }

            if (password.length > 0 && password.length < 8)
            {
                saveButton.addClass("ui-state-disabled");
                saveButton.prop("disabled", true);
                statusElement.text(LangSupport.getLangString("wifiPasswordTooShort") || "Password too short (min 8 characters)");
                statusElement.css("color", "#F44336");
                return false;
            }

            if (password.length === 0 && passwordConfirm.length === 0)
            {
                saveButton.removeClass("ui-state-disabled");
                saveButton.prop("disabled", false);
                statusElement.text(LangSupport.getLangString("wifiPasswordOpenMode") || "Open WiFi selected (no password)");
                statusElement.css("color", "#4CAF50");
                return true;
            }

            if (!passwordConfirm || passwordConfirm.length === 0)
            {
                saveButton.addClass("ui-state-disabled");
                saveButton.prop("disabled", true);
                statusElement.text(LangSupport.getLangString("wifiPasswordRepeatPrompt") || "Please repeat the password");
                statusElement.css("color", "#FF9800");
                return false;
            }

            if (password !== passwordConfirm)
            {
                saveButton.addClass("ui-state-disabled");
                saveButton.prop("disabled", true);
                statusElement.text(LangSupport.getLangString("wifiPasswordMismatch") || "Passwords do not match");
                statusElement.css("color", "#F44336");
                return false;
            }

            saveButton.removeClass("ui-state-disabled");
            saveButton.prop("disabled", false);
            statusElement.text(LangSupport.getLangString("wifiPasswordMatch") || "Passwords match");
            statusElement.css("color", "#4CAF50");
            return true;
        };

        passwordInput.on("input", validate);
        passwordConfirmInput.on("input", validate);
        $("#popupWifiPassword").on("popupafteropen", validate);
        validate();
    },

    setWifiPassword : function ()
    {
        var passwordInput = $("#wifiPasswordInput");
        var passwordConfirmInput = $("#wifiPasswordConfirmInput");
        var statusElement = $("#wifiPasswordStatus");
        var password = passwordInput.val();
        var passwordConfirm = passwordConfirmInput.val();

        // Validate password
        if (password.length > 63)
        {
            statusElement.text(LangSupport.getLangString("wifiPasswordTooLong") || "Password too long (max 63 characters)");
            statusElement.css("color", "#F44336");
            return;
        }

        if (password.length > 0 && password.length < 8)
        {
            statusElement.text(LangSupport.getLangString("wifiPasswordTooShort") || "Password too short (min 8 characters)");
            statusElement.css("color", "#F44336");
            return;
        }

        if (password !== passwordConfirm)
        {
            statusElement.text(LangSupport.getLangString("wifiPasswordMismatch") || "Passwords do not match");
            statusElement.css("color", "#F44336");
            return;
        }

        if (this.enableServerCom)
        {
            var jsonPasswordObj = {"password": password};
            $.ajax({url: "/setWifiPassword", type: "POST", dataType: "json", data: JSON.stringify(jsonPasswordObj), 
                    contentType: "application/json"})
            .done(function(jsonResult){
                if (jsonResult.success)
                {
                    if (password.length === 0)
                    {
                        statusElement.text(LangSupport.getLangString("wifiPasswordOpenSaved") || "Open WiFi saved successfully");
                    }
                    else
                    {
                        statusElement.text(LangSupport.getLangString("wifiPasswordSaved") || "WiFi password saved successfully");
                    }
                    statusElement.css("color", "#4CAF50");
                    passwordInput.val("");
                    passwordConfirmInput.val("");
                    $("#wifiPasswordButton").addClass("ui-state-disabled").prop("disabled", true);
                    $("#wifiPasswordButton").blur();
                    $("#popupWifiPassword").popup("close");
                }
                else
                {
                    statusElement.text(jsonResult.message || "Error saving WiFi password");
                    statusElement.css("color", "#F44336");
                }
            })
            .fail(function(){
                statusElement.text(LangSupport.getLangString("wifiPasswordFailed") || "Failed to save WiFi password");
                statusElement.css("color", "#F44336");
            });
        }
        else
        {
            statusElement.text(LangSupport.getLangString("wifiPasswordDummy") || "WiFi password saved (Dummy mode)");
            statusElement.css("color", "#4CAF50");
            passwordInput.val("");
            passwordConfirmInput.val("");
            $("#wifiPasswordButton").addClass("ui-state-disabled").prop("disabled", true);
            $("#popupWifiPassword").popup("close");
        }
    },

    getWifiPassword : function ()
    {
        if (this.enableServerCom)
        {
            $.ajax({url: "/getWifiPassword", type: "GET", dataType: "json"})
            .done(function(jsonResult){
                var statusElement = $("#wifiPasswordStatus");
                if (jsonResult.success && jsonResult.hasPassword)
                {
                    statusElement.text(LangSupport.getLangString("wifiPasswordExists") || "WiFi password is set");
                    statusElement.css("color", "#4CAF50");
                }
                else if (jsonResult.success && jsonResult.hasConfig)
                {
                    statusElement.text(LangSupport.getLangString("wifiPasswordOpenModeConfigured") || "Open WiFi is configured");
                    statusElement.css("color", "#4CAF50");
                }
                else
                {
                    statusElement.text(LangSupport.getLangString("wifiPasswordNotSet") || "WiFi password not yet set");
                    statusElement.css("color", "#FF9800");
                }
            })
            .fail(function(){
                console.log("Error: Could not retrieve WiFi password status");
            });
        }
        else
        {
            var statusElement = $("#wifiPasswordStatus");
            statusElement.text(LangSupport.getLangString("wifiPasswordDummyMode") || "Running in Dummy mode");
            statusElement.css("color", "#2196F3");
        }
    }
};