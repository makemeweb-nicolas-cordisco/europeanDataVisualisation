/**
 * @file
 */

function updateChart(){
  jQuery(document).ready(function ($) {
    var year = $('#year').val();
    var countriesValues = [];
    $('#country-select input:not(#all-countries):checked').each(function (index) {
        countriesValues.push($(this).val());
    });
    var dataChart = [];
    var euDataValue = 0;
    $.each(data.data[year], function (countryCode,countryData) {
      if ($.inArray(countryCode, countriesValues) >= 0) {
          var obj = jQuery.extend({}, countryData);
          dataChart.push(obj);
      }
      else {
          // Countries not selected.
          euDataValue = euDataValue + parseFloat(countryData.value);
      }
    });
    // Add additionned values of others country at the end of data.
    if (euDataValue > 0) {
        Drupal.settings.mmw_charts.euData.value = euDataValue;
        var obj = jQuery.extend({}, Drupal.settings.mmw_charts.euData);
        dataChart.push(obj);
    }

    var newData = jQuery.extend({}, data);
    newData.data = dataChart;
    chart.setJSONData(newData);
  });
}
