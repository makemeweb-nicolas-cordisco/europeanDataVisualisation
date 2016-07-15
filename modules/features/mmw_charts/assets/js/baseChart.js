/**
 * @file
 */

jQuery(document).ready(function($) {
  if (Drupal.settings.mmw_charts.parameters.commission) {
      buildChartCommission();
  }
  else {
      buildChartLocal();
  }

    // If type and themes already selected then update data and chart.
  if ($('#chart-types').val() != "" && $('#themes input:checkbox:checked').length > 0) {
      $('#countries-chart-container').show();
      $('.timeline').show();
      $('#themes input:checked:first').trigger('change');
      hideGraphForUnit($('#themes-lvl1 option:selected').data('unit'));
  }
});

function buildChartLocal() {
  chart = new FusionCharts({
      type: Drupal.settings.mmw_charts.parameters.type,
      renderAt: 'chart-container',
      width: Drupal.settings.mmw_charts.parameters.width,
      height: Drupal.settings.mmw_charts.parameters.height,
      dataFormat: 'json',
      dataEmptyMessage: " ",
      containerBackgroundOpacity: '0',
      events: {
          "renderComplete": HideAxis
      }
    });
  chart.render();
}

function buildChartCommission() {
  chart = $wt.charts.render({
      type: Drupal.settings.mmw_charts.parameters.type,
      renderAt: 'chart-container',
      width: Drupal.settings.mmw_charts.parameters.width,
      height: Drupal.settings.mmw_charts.parameters.height,
      dataFormat: 'json',
      dataEmptyMessage: " ",
      containerBackgroundOpacity: '0',
      dataSource: {
          "chart": {
              "theme": "fint"
            }
      },
      events: {
          "renderComplete": HideAxis
      }
    });
}

function HideAxis(){
    // if(jQuery('#chart-types').val()=='scrollcombidy2d'){
    //     var countriesValues = [];
    //     jQuery('#country-select input:not(#all-countries):checked').each(function(index) {
    //         countriesValues.push(jQuery(this).val());
    //     });
    //
    //     if(countriesValues.indexOf('EU28')==(-1)){
    //         jQuery('.fusioncharts-yaxis-0-gridlabels').hide();
    //         jQuery('.fusioncharts-yaxis-1-gridlabels').show();
    //         jQuery('.fusioncharts-yaxis-0-title').hide();
    //         jQuery('.fusioncharts-yaxis-1-title').show();
    //     }else{
    //         jQuery('.fusioncharts-yaxis-0-gridlabels').show();
    //         jQuery('.fusioncharts-yaxis-0-title').show();
    //         if(countriesValues.length==1){
    //             jQuery('.fusioncharts-yaxis-1-gridlabels').hide();
    //             jQuery('.fusioncharts-yaxis-1-title').hide();
    //         }
    //     }
    // }
}