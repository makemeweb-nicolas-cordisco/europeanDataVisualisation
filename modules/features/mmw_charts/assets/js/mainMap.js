/**
 * @file
 */

function updateChart(){
  jQuery(document).ready(function ($) {
    var year = $('#year').val();
    var newData = jQuery.extend({}, data);
    newData.data = data.data[year];
    // entityDef defined in main.js.
    newData.entityDef = entityDefs;

    // Disable hover on no EU country
    // countriesLeft defined in main.js.
    $.each(countriesLeft, function (index, val) {
        newData.data.push({id: val,value: null,useHoverColor: "0", showToolTip: "0"});
    });
    chart.setJSONData(newData);
  });
}
