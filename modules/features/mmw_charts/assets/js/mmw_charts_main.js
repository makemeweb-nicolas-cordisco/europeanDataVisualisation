/**
 * @file
 */

var data = null;
var categories = null;
var chart = null;

var years = null;
var playing = false;
var index = 1;
var selectLvl1Options;
var chartTypeOptions;
// countriesLeft is used in mini-map.js, main.js and mainMap.js.
var countriesLeft = ["ru","ua","tk","by","md","mk","cs","al","ko","mo","ch","no","is","rd","ba"];

// Remaping country ISO to FusionmapChart internal ID
// used for mini map and chart maps/europe2.
var entityDefs = [];
function hideGraphForUnit() {
  jQuery('#chart-types').html(chartTypeOptions);
  jQuery("#chart-types option").show();
  if(jQuery("#themes input:checked[data-limited=1]").length>0){
    jQuery("#chart-types option[data-limited=1]").remove();
  }
}
jQuery(document).ready(function ($) {

    //for IE remove hidden option because it is impossible to hide option in ie
    selectLvl1Options = $('#themes-lvl1').html();
    chartTypeOptions = $('#chart-types').html();
    $('#themes-lvl1 .hidden').remove();
    
    $('#themes-lvl1').change(function(evt){
      $('#themes .main-theme-container input').prop('checked', false);
      $('#themes .main-theme-container.' + $(this).val() + ' input:first').prop('checked', true);
      $('#themes .main-theme-container').hide();
      $('#themes .main-theme-container.' + $(this).val()).show();
      
      if ($('#chart-types').val() != "") {
          $('#countries-chart-container').show();
          $('.timeline').show();
          $('.charts-landing-img').hide();
          $('#themes .main-theme-container.' + $(this).val() + ' input:first').trigger('change');
      }
      $('#chart-types').removeProp('disabled');
    });
    
    $('#themes-lvl0').change(function(evt){
        $('#themes-lvl1').html(selectLvl1Options);
        $('#themes-lvl1 option[value!=""]').removeAttr('selected').addClass('hidden');
        $('#themes-lvl1 option[value=""]').prop('selected', true);
        $('#themes-lvl1 option.'+$(this).val()).removeClass('hidden');
        $('.main-theme-container').hide();
        $('.timeline').hide();
        $('.charts-landing-img').show();
        $('#countries-chart-container').hide();
        $('#themes-lvl1 .hidden').remove();
        $('#themes-lvl1').removeAttr('disabled');
        
    });

    $('#themes input').change(function(evt){
      if (Drupal.settings.mmw_charts.parameters && !Drupal.settings.mmw_charts.parameters['categories_multiselect']) {
          $('#themes input:checkbox:checked:not(#' + $(this).attr('id') + ')').removeAttr('checked');
      }
        var themesValues = $('#themes input:checkbox:checked').map(function() {
            return this.value;
        }).get();
        if (themesValues.length > 0 && $('#chart-types').val() != "") {
            // Defined in main.js.
            updateData(themesValues);
        }
        // If data not compatible with graph type.
        if ($('#chart-types').val() != "" && $("#chart-types option:selected").data('limited') && $("#themes input:checked[data-limited=1]").length>0) {
          $('#chart-types').val("");
            $('.main-theme-container').hide();
            $('.timeline').hide();
            $('.charts-landing-img').show();
            $('#countries-chart-container').hide();
            $('#graph-select-error-message').show();
        }
        hideGraphForUnit();
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

    $('.lvl4-button').click(function(evt){
        $('.menu-lvl4').toggle();
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

    if (typeof Drupal.settings.mmw_charts.parameters !== 'undefined') {
      if (Drupal.settings.mmw_charts.parameters['select_country']) {
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
      if($('#country-select').length) {
          $('#show-country-select-mmw-charts').click(function(e) {
              var link_country_mobile = $(this);
              e.preventDefault();
              if($('#country-select').hasClass('hidden-xs')) {
                $('#country-select').removeClass('hidden-xs').hide().slideToggle('slow', function(){
                    link_country_mobile.find('span').text('Hide countries');
                });
              }
              else {
                $('#country-select').slideToggle('slow', function(){
                    $('#country-select').addClass('hidden-xs');
                    link_country_mobile.find('span').text('Show countries');
                });
              }
          });
      }
      if (Drupal.settings.mmw_charts.parameters['timeline']) {
          $('#year').change(function(evt){
              $('.countdown-year').html($(this).val());
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
                      update(Drupal.settings.mmw_charts.parameters['time_delay']);
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
