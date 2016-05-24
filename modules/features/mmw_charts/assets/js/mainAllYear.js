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
    $.each(countriesValues, function(index,countryCode) {
        $.each(themesValues, function(index, theme) {
            var obj = jQuery.extend({}, data.dataset[countryCode][theme]);
            dataset.push(obj);
        });
    });

    var newData = jQuery.extend({}, data);
    newData.dataset = dataset;
    chart.setJSONData(newData);
  });
}
