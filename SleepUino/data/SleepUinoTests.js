var TestRunner = {
    listEntry : '<li><a href="#tests" id="tabTests">Tests</a></li>',
    htmlTests : `
        <div id="tests">
            <div class="ui-body ui-body-a ui-corner-all">
                <h1>SleepUINOTests</h1>
                <div>
                    <ol id="testList" data-role="listview" data-inset="true" data-divider-theme="a">
                        <li data-role="list-divider">Test WakeListFunc</li>
                        <li class="testRun" id="testGetMinutesPerDay"><a href="#">GetMinutesPerDay</a></li>
                        <li class="testRun" id="testGetWakeUpTimeFunc"><a href="#">GetWakeUpTimeFunc</a></li>
                        <li class="testRun" id="testGetGoBedTimeFunc"><a href="#">GetGoBedTimeFunc</a></li>
                        <li class="testRun" id="testCreateListEntry_AlarmOn"><a href="#">CreateListEntry_AlarmOn</a></li>
                        <li class="testRun" id="testCreateListEntry_AlarmOff"><a href="#">CreateListEntry_AlarmOff</a></li>
                        <li class="testRun" id="testGetAlarmValueFunc_AlarmOn"><a href="#">GetAlarmValueFunc_AlarmOn</a></li>
                        <li class="testRun" id="testGetAlarmValueFunc_AlarmOff"><a href="#">GetAlarmValueFunc_AlarmOff</a></li>
                        <li class="testRun" id="testGetWakeUpListTargetFormat"><a href="#">GetWakeUpListTargetFormat</a></li>
                    </ol>
                </div>
                <div>
                    <input type="button" id="testAll" value="Run all tests" onclick="TestRunner.runAllTests()">      
                </div>
            </div>
        </div>
    `, 

    addTestTab : function()
    {
        $("#navbar ul").append(TestRunner.listEntry);
        
        $(TestRunner.htmlTests).appendTo("#tabs");
        $("#tabs").tabs("refresh");
        $("#navbar").navbar();
        $("#tests").trigger('create');

        //Subscribtion for Events
        $('#testList').on('click', '.testRun', function(){
            TestRunner.runTest($(this));
        });
    },

    runTest : function(listEntry)
    {
        var testFunctionName = listEntry.attr('id');

        var result = window["Tests"][testFunctionName]();
    
        if (result)
        {
            (listEntry.children().eq(0)).removeClass("ui-btn-a ui-btn-b").addClass("ui-btn-c");
        }
        else
        {
            (listEntry.children().eq(0)).removeClass("ui-btn-a ui-btn-c").addClass("ui-btn-b");
        }
    },

    runAllTests : function()
    {
        $('.testRun').each(function(index){
            TestRunner.runTest($(this));
        })
    }
}

var Tests = {
    
    testGetWakeUpListTargetFormat : function()
    {

        console.log("06:00" + WakeListFunc.getWakeUpListTargetFormat(WakeListFunc.getMinutesPerDay("06:00"), 3));
        console.log("09:00" + WakeListFunc.getWakeUpListTargetFormat(WakeListFunc.getMinutesPerDay("09:00"), 1));
        console.log("12:00" + WakeListFunc.getWakeUpListTargetFormat(WakeListFunc.getMinutesPerDay("12:00"), 2));
        console.log("15:00" + WakeListFunc.getWakeUpListTargetFormat(WakeListFunc.getMinutesPerDay("15:00"), 1));

        var funcRestult = WakeListFunc.getWakeUpListTargetFormat(360, 2);
        const expectedResult = 5762;
        return (funcRestult == expectedResult);
    },

    testGetMinutesPerDay : function()
    {
        var funcResult = WakeListFunc.getMinutesPerDay("06:10");
        return (funcResult == 370);
    },

    testGetWakeUpTimeFunc : function()
    {
        var text = "|🌞 06:00 | 🌜 09:00 | ⏰ |" 
        var funcResult = WakeListFunc.getWakeUpTimeFunc(text);
        return (funcResult == "06:00");
    },

    testGetGoBedTimeFunc : function()
    {
        var text = "|🌞 06:00 | 🌜 09:00 | ⏰ |" 
        var funcResult = WakeListFunc.getGoBedTimeFunc(text);
        return (funcResult == "09:00");
    },

    testCreateListEntry_AlarmOff : function()
    {
        var text = "|&#127774;&nbsp;04:00&nbsp;|&nbsp;&#127772;&nbsp;05:00&nbsp;|&nbsp;<s>&#x23F0;</s>&nbsp;|";
        var expectedResult = "<li class='wakeListItem'><a class='wakeTimeItem'>" + text + "</a><a class='deleteMe'></a></li>";

        var funcResult = WakeListFunc.createListEntry("04:00", "05:00", false);
        return (funcResult == expectedResult);
    },

    testCreateListEntry_AlarmOn : function()
    {
        var text = "|&#127774;&nbsp;04:00&nbsp;|&nbsp;&#127772;&nbsp;05:00&nbsp;|&nbsp;&#x23F0;&nbsp;|";
        var expectedResult = "<li class='wakeListItem'><a class='wakeTimeItem'>" + text + "</a><a class='deleteMe'></a></li>";

        var funcResult = WakeListFunc.createListEntry("04:00", "05:00", true);
        return (funcResult == expectedResult);
    },

    testGetAlarmValueFunc_AlarmOn : function()
    {
        var html = "|&#127774;&nbsp;04:00&nbsp;|&nbsp;&#127772;&nbsp;05:00&nbsp;|&nbsp;<s>&#x23F0;</s>&nbsp;|";
        var expectedResult = true;

        var funcResult = WakeListFunc.getAlarmValueFunc(html);
        return (funcResult == expectedResult);
    },
    
    testGetAlarmValueFunc_AlarmOff : function()
    {
        var html = "|&#127774;&nbsp;04:00&nbsp;|&nbsp;&#127772;&nbsp;05:00&nbsp;|&nbsp;&#x23F0;&nbsp;|";
        var expectedResult = false;

        var funcResult = WakeListFunc.getAlarmValueFunc(html);
        return (funcResult == expectedResult);
    }
};