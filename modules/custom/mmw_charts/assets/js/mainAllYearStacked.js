/**
 * @file
 */

function updateChart(){
  jQuery(document).ready(function ($) {
    var themesValues = $('#themes input:checkbox:checked').map(function() {
        return this.value;
    }).get();
    var countriesValues = [];
    $('#country-select input:not(#all-countries):checked').each(function(index) {
        countriesValues.push($(this).val());
    });
    var dataset = [];
    var euDataValue = {};

    $.each(data.dataset, function(countryCode,countryData) {
        $.each(countryData, function(themeIndex, themeData) {
          if ($.inArray(countryCode, countriesValues) >= 0) {
              var obj = jQuery.extend({}, themeData);
              dataset.push(obj);
          }
          else {
              // Countries not selected.
            if (countryCode != 'eu28' && countryCode != 'eu27') {
                // If first countries not selected.
              if (!(themeIndex in euDataValue)) {
                  euDataValue[themeIndex] = jQuery.extend(true,{}, themeData);
                  euDataValue[themeIndex].color = euData.color;

                  // Replace country code of initial data by 'EU'.
                  euDataValue[themeIndex].seriesname = euDataValue[themeIndex].seriesname.slice(0, -2) + " EU";

                  $.each(euDataValue[themeIndex].data, function(index, yearData) {
                      var str = yearData.toolText;
                      var firstIndex = str.indexOf('{br}Country') + 12;
                      var secondIndex = str.indexOf('{br}Year');
                      var strToReplace = str.substring(firstIndex,secondIndex);
                      yearData.toolText = str.replace(strToReplace,euData.label);
                  });
              }
              else {
                    // Add countries data to "other countries" data.
                    $.each(euDataValue[themeIndex].data, function(index, yearData) {
                        yearData.value = parseFloat(yearData.value) + parseFloat(themeData.data[index].value);
                    });
              }
            }
          }
        });
    });
    // Add additionned values of others country at the end of data.
    $.each(euDataValue, function(index, themeData) {
        dataset.push(themeData);
    });
    // console.log(dataset);
    var newData = jQuery.extend({}, data);
    newData.dataset = dataset;
    chart.setJSONData(newData);
  });
}
