/**
 * @file
 */

jQuery(document).ready(function ($) {
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
  FusionCharts.ready(function () {
      chart = new FusionCharts({
          type: Drupal.settings.mmw_charts.parameters.type,
          renderAt: 'chart-container',
          width: Drupal.settings.mmw_charts.parameters.width,
          height: Drupal.settings.mmw_charts.parameters.height,
          dataFormat: 'json',
          dataEmptyMessage: " ",
          containerBackgroundOpacity: '0'
        });
      chart.render();
  });

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
        }
    });
}
