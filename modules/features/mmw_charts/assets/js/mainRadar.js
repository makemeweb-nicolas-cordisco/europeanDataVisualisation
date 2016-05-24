/**
 * @file
 */

function updateChart(){
  jQuery(document).ready(function ($) {
    var year = $('#year').val();
    var countriesValues = [];
    $('#country-select input:not(#all-countries):checked').each(function(index) {
        countriesValues.push($(this).val());
    });
    var themesValues = $('#themes input:checkbox:checked').map(function() {
        return this.value;
    }).get();
    var dataChart = [];
    var categoriesChart = [];

    $.each(countriesValues, function(countryIndex,countryCode) {
        categoriesChart.push(data.categories[countryCode]);
        $.each(themesValues, function(themeIndex, theme) {
          if (!(themeIndex in dataChart)) {
              dataChart[themeIndex] = {data:[],seriesname:""};
          }

            dataChart[themeIndex].data.push(data.dataset[theme][year][countryCode].data[0]);
            dataChart[themeIndex].seriesname = data.dataset[theme].seriesname;
        });
    });

    var newData = jQuery.extend({}, data);
    newData.dataset = dataChart;
    newData.categories = [{"category":categoriesChart}];
    chart.setJSONData(newData);
  });
}
