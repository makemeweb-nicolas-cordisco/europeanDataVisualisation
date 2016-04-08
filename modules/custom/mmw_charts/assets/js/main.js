/**
 * @file
 */

var years = null;
var playing = false;
var index = 1;
// countriesLeft is used in mini-map.js, main.js and mainMap.js.
var countriesLeft = ["ru","ua","tk","by","md","mk","cs","al","ko","mo","ch","no","is","rd","ba"];

// Remaping country ISO to FusionmapChart internal ID
// used for mini map and chart maps/europe2.
var entityDefs = [];
function hideGraphForUnit(unit)
{
  jQuery("#chart-types option").show();
  jQuery("#chart-types option[data-units-excluded~='" + unit + "']").hide();
}
jQuery(document).ready(function ($) {

    $('#themes-lvl1').change(function(evt){
        $('#themes .main-theme-container input').prop('checked', false);
        $('#themes .main-theme-container.' + $(this).val() + ' input:first').prop('checked', true);

        var unit = $(this).find('option:selected').data('unit');
        // If data not compatible with graph type.
      if ($('#chart-types').val() != "" && $("#chart-types option:selected").data('units-excluded').indexOf(unit) != -1) {
          $('#chart-types').val("");
          $('.main-theme-container').hide();
          $('.timeline').hide();
          $('#countries-chart-container').hide();
          $('#graph-select-error-message').show();
      }
        hideGraphForUnit(unit);
      if ($('#chart-types').val() != "") {
          $('#countries-chart-container').show();
          $('.timeline').show();
          $('#themes .main-theme-container').hide();
          $('#themes .main-theme-container.' + $(this).val()).show();
          $('#themes .main-theme-container.' + $(this).val() + ' input:first').trigger('change');
      }
    });

    $('#themes input').change(function(evt){
      if (!parameters['categories_multiselect']) {
          $('#themes input:checkbox:checked:not(#' + $(this).attr('id') + ')').removeAttr('checked');
      }
        var themesValues = $('#themes input:checkbox:checked').map(function() {
            return this.value;
        }).get();
        if (themesValues.length > 0) {
            // Defined in main.js.
            updateData(themesValues);
        }
    });

    var jsonString = $('.data-map').val();
    if (jsonString) {
        var jsonObj = JSON.parse($('.data-map').val());
        $.each(jsonObj, function(index, dataCountry) {
            var internalIdValue = "EU." + dataCountry.id;

            // Exeption for fusion chart internal id.
          if (dataCountry.id == 'GB') {
            internalIdValue = "EU.UK";
          }
          if (dataCountry.id == 'SK') {
            internalIdValue = "EU.SL";
          }
          if (dataCountry.id == 'HR') {
            internalIdValue = "EU.HY";
          }
          if (dataCountry.id == 'EL') {
            internalIdValue = "EU.GR";
          }

            var obj = {
                internalId: internalIdValue,
                newId: dataCountry.id,
                color: dataCountry.color
            };
            entityDefs.push(obj);
        });
        $.each(countriesLeft, function(index, dataCountry) {
            var obj = {
                internalId: "EU." + dataCountry.toUpperCase(),
                newId: dataCountry.toUpperCase(),
                color: ""
            };
            entityDefs.push(obj);
        });
    }

    $(".btn-print").click(function() {
        window.print();
    });

    $('.lvl3-button').click(function(evt){
        $('.menu-lvl3').toggle();
      if ($(this).text() == "+") {
          $(this).text("-");
      }
      else {
          $(this).text("+");
      };
    });

    $('#chart-types').change(function(evt){
        var val = $(this).val();
        var themesValues = "";
      if ($('#themes input:checkbox:checked')) {
          themesValues = $('#themes input:checkbox:checked').map(function() {
              return this.value;
          }).get();
      }

      if (val != "") {
          var url = '/' + Drupal.settings.mmw_charts.url_alias + '?type=' + val + '&themes=' + themesValues;
          window.location = url;
      }
    });

    $('#scenario').change(function(evt) {
        var val = $(this).val();
      if (val != '_none') {
          var themesValues = "";
          var typeValues = "";
        if ($('#chart-types input:checkbox:checked')) {
            typeValues = $('#chart-types').val();
        }
        if ($('#themes input:checkbox:checked')) {
            themesValues = $('#themes input:checkbox:checked').map(function() {
                return this.value;
            }).get();
        }
          var url = '/' + $(this).val() + '?type=' + typeValues + '&themes=' + themesValues;
          window.location = url;
      }
    });

    if (typeof parameters !== 'undefined') {
      if (parameters['select_country']) {
          $('#country-select input:not(#all-countries)').change(function(evt){
              updateChart();
          });

            $('#all-countries').change(function(evt){
              if ($(this).prop('checked')) {
                  $('#country-check-eu28').removeAttr('checked');
                  $('#country-select input:not(#all-countries, #country-check-eu28)').prop('checked',true);
              }
              else {
                  $('#country-select input:not(#all-countries, #country-check-eu28)').removeAttr('checked');
              }
                updateChart();
            });
      }

      if (parameters['timeline']) {
          $('#year').change(function(evt){
              $('#absolute-year').html($(this).val());
              updateChart();
          });

            years = $('#year option').map(function() {
                return this.value;
            }).get();

            $(".slider")
            // Activate the slider with options.
              .slider({
                animate: true,
                min: 0,
                max: years.length - 1,
                value: $('#year option:selected').index()
              })
              // Add pips with the labels set to "months".
                .slider("pips", {
                    rest: "label",
                    labels: years
                }).on("slidechange", function(e,ui) {
                    index = ui.value + 1;

                    $('#year option').removeAttr('selected');
                    $('#year option[value=' + years[ui.value] + ']').prop('selected',true);
                    $('#year').trigger('change');
                });

                $('#play').click(function(evt){
                    var icon = $('#play').find('i');

                  if (playing) {
                      playing = false;
                      icon.removeClass('fa-stop');
                      icon.addClass('fa-play');
                  }
                  else {
                      playing = true;
                      icon.removeClass('fa-play');
                      icon.addClass('fa-stop');

                    if (index >= years.length) {
                      if ($('#year option:selected').index() != 0) {
                          index = 0;
                      }
                    }

                      $(".slider").slider("value", index);
                      update(parameters['time_delay']);
                  }
                });
      }
    }

    function updateData(themesValue){
        var param = {
            "type" : $('#chart-types').val(),
            "nid" : Drupal.settings.mmw_charts.node_id,
            "themes" : themesValue
        };
        $.getJSON($('#ajax-url').attr('href'),param,function(result){
            data = result.data;
            updateChart();
        });
    }

    function update(time){
        var icon = $('#play').find('i');
        setTimeout(function(){
          if (playing) {
            $(".slider").slider("value", index);
            if (index < years.length) {
                update(time,index);
            }
            else {
                playing = false;
                icon.removeClass('fa-stop');
                icon.addClass('fa-play');
            }
          }
        }, time);
    }

});
