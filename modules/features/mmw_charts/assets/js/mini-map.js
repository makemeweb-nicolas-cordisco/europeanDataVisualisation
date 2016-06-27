/**
 * @file
 */

var mapDataSource;
var mapChart;

jQuery(document).ready(function ($) {
    mapDataSource = {
        chart: {
            caption: "European Union",
            theme: "fint",
            formatNumberScale: 0,
            nullEntityColor: "#e1e1e1",
            animation: 1,
            showLegend: 0
        },
        // entityDef defined in main.js.
        entityDef: entityDefs,
        data: $('.data-map').val()
    };

  if (Drupal.settings.mmw_charts.parameters.commission) {
      buildMapCommission($);
  }
  else {
      buildMapLocal($);
  }

    $('#country-select').change(function () {
        mapDataSource.data = [];
        $(this).find("input:checked").each(function () {
            var regex = /^[A-Z]{2}$/g;
          if ($(this).val().match(regex)) {
              mapDataSource.data.push({
                  id: $(this).val(),
                  value: ($(this).prop('checked') ? "" : 0),
                  color: $(this).data('color'),
                  tooltext: ""
                });
          }

            // Disable hover on no EU country
            // countriesLeft defined in main.js.
            $.each(countriesLeft, function (index, val) {
                mapDataSource.data.push({id: val, value: null, useHoverColor: "0", showToolTip: "0"});
            });
        });
        mapChart.setJSONData(mapDataSource);
    });
});

function buildMapLocal($) {
  FusionCharts.ready(function () {
      mapChart = new FusionCharts({
          type: 'maps/europe2',
          renderAt: 'map-chart',
          width: '100%',
          height: '250',
          showBorder: 0,
          dataFormat: 'json',
          showLegend: false,
          dataSource: mapDataSource,
          events: {
              entityClick: function (evt, data) {
                  var iso = data.id.replace('EU.', "");
                  $('#country-select').find("input[value=" + iso.toUpperCase() + "]").click();
              },
              rendered: function (eventObj, dataObj) {
                  $('#country-select').trigger('change');
              }
        }
        });
      mapChart.render();
  });

}

function buildMapCommission($) {
  mapChart = $wt.charts.render({
      type: 'maps/europe2',
      renderAt: 'map-chart',
      width: '250',
      height: '250',
      showBorder: 0,
      dataFormat: 'json',
      showLegend: false,
      dataSource: mapDataSource,
      events: {
          entityClick: function (evt, data) {
              var iso = data.id.replace('EU.', "");
              $('#country-select').find("input[value=" + iso.toUpperCase() + "]").click();
          },
          rendered: function (eventObj, dataObj) {
              $('#country-select').trigger('change');
          }
    }
    });
}
