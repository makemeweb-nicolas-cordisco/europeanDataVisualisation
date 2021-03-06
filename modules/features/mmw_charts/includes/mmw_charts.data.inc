<?php

/**
 * @file
 * MMW Charts data settings function file.
 *
 * @brief Data settings for the charts, each charts type in its own settings.
 *
 * @package mmw_charts
 *
 * @author Makemeweb
 */

/**
 * Array of value for rendering charts.
 *
 * @param string $nid
 *   The node id of current page.
 *
 * @return array $static_data
 *   All data for the differents charts type.
 */
function _mmw_charts_get_static_data($nid) {
  $colors = array(
    "eu28" => "#FD8433",
    "eu27" => "#00FF00",
    "au" => "#34495E",
    "be" => "#A2C83F",
    "bg" => "#CE7057",
    "cp" => "#67B9CE",
    "cz" => "#CCB394",
    "dk" => "#B39147",
    "es" => "#275680",
    "fi" => "#CE8552",
    "fr" => "#F59D1F",
    "ge" => "#C23A2C",
    "gr" => "#1AB99B",
    "hu" => "#894B9E",
    "ir" => "#93A5A5",
    "it" => "#315239",
    "la" => "#4ABA70",
    "li" => "#CC66A5",
    "lx" => "#802F35",
    "ma" => "#805B53",
    "nl" => "#4B8056",
    "pd" => "#8798CC",
    "pl" => "#2A2A2A",
    "ro" => "#3597D4",
    "sk" => "#2481BC",
    "sn" => "#81CCB8",
    "sp" => "#86B6CD",
    "sv" => "#E84C3D",
    "uk" => "#7F6E5E",
    "hr" => "#F0C514",
  );

  $mapping = array(
    "eu28" => "EU28",
    "eu27" => "EU27",
    "au" => "AT",
    "be" => "BE",
    "bg" => "BG",
    "cp" => "CY",
    "cz" => "CZ",
    "dk" => "DK",
    "es" => "EE",
    "fi" => "FI",
    "fr" => "FR",
    "ge" => "DE",
    "gr" => "EL",
    "hu" => "HU",
    "ir" => "IE",
    "it" => "IT",
    "la" => "LV",
    "li" => "LT",
    "lx" => "LU",
    "ma" => "MT",
    "nl" => "NL",
    "pd" => "PL",
    "pl" => "PT",
    "ro" => "RO",
    "sk" => "SK",
    "sn" => "SI",
    "sp" => "ES",
    "sv" => "SE",
    "uk" => "UK",
    "hr" => "HR",
  );

  $theme = "fint";
  $xaxisname = 'Year';
  $yaxisname = 'Unit';

  $graph_types = array(
    "radar" => array(
      "name" => "Radar chart",
      "categories_multiselect" => TRUE,
      "type" => "radar",
      "select_country" => TRUE,
      "countries_exclude" => array('eu27'),
      "countries_selected" => "all",
      "limited" => FALSE,
      "timeline" => TRUE,
      "time_delay" => 5000,
      "custom_js" => "mainRadar.js",
      "process_data_function" => "_mmw_charts_get_json_radar",
      "chart_options" => array(
        "numberPrefix" => "",
        "bgColor" => "#ffffff",
        "bgAlpha" => "100",
        "canvasBgAlpha" => "100",
        "theme" => $theme,
        "exportEnabled" => '1',
        "exportAtClientSide" => '1',
      ),
    ),
    "msline" => array(
      "name" => "Line graph",
      "categories_multiselect" => TRUE,
      "type" => "msline",
      "select_country" => TRUE,
      "countries_exclude" => array('eu27'),
      "countries_selected" => array('eu28'),
      "limited" => FALSE,
      "timeline" => FALSE,
      "time_delay" => 5000,
      "custom_js" => "mainAllYear.js",
      "process_data_function" => "_mmw_charts_get_json_all_year",
      "chart_options" => array(
        "subcaption" => "",
        "xaxisname" => $xaxisname,
        "yaxisname" => $yaxisname,
        "lineThickness" => "2",
        "labelHeight" => "30",
        "bgColor" => "#ffffff",
        "bgAlpha" => "100",
        "canvasBgAlpha" => "100",
        "showValues" => "0",
        "theme" => $theme,
        "exportEnabled" => '1',
        "exportAtClientSide" => '1',
      ),
    ),
    "doughnut2d" => array(
      "name" => "Pie chart",
      "categories_multiselect" => FALSE,
      "type" => "doughnut2d",
      "select_country" => TRUE,
      "countries_exclude" => array('eu27', 'eu28'),
      "countries_selected" => "all",
      "limited" => TRUE,
      "timeline" => TRUE,
      "time_delay" => 5000,
      "custom_js" => "mainCategoriesSingleSelect.js",
      "process_data_function" => "_mmw_charts_get_json_single_select",
      "chart_options" => array(
        "numberPrefix" => "",
        "showPercentValues" => "1",
        "showPercentInTooltip" => "0",
        "enableMultiSlicing" => "1",
        "decimals" => "1",
        "is2D" => "0",
        "bgColor" => "#ffffff",
        "bgAlpha" => "100",
        "canvasBgAlpha" => "100",
        "showBorder" => "0",
        "showValues" => "1",
        "startingAngle" => "320",
        "theme" => $theme,
        "exportEnabled" => '1',
        "exportAtClientSide" => '1',
      ),
    ),
    "pyramid" => array(
      "name" => "Pyramid chart",
      "categories_multiselect" => FALSE,
      "type" => "pyramid",
      "select_country" => TRUE,
      "countries_exclude" => array('eu27', 'eu28'),
      "countries_selected" => "all",
      "limited" => TRUE,
      "timeline" => TRUE,
      "time_delay" => 5000,
      "custom_js" => "mainCategoriesSingleSelect.js",
      "process_data_function" => "_mmw_charts_get_json_single_select",
      "chart_options" => array(
        "numberPrefix" => "",
        "showPercentValues" => "1",
        "showPercentInTooltip" => "0",
        "enableMultiSlicing" => "1",
        "decimals" => "1",
        "is2D" => "0",
        "bgColor" => "#ffffff",
        "bgAlpha" => "100",
        "canvasBgAlpha" => "100",
        "showBorder" => "0",
        "theme" => $theme,
        "exportEnabled" => '1',
        "exportAtClientSide" => '1',
      ),
    ),
    "scrollcombidy2d" => array(
      "name" => "Bar charts + line chart mixed",
      "categories_multiselect" => TRUE,
      "type" => "scrollcombidy2d",
      "select_country" => TRUE,
      "countries_exclude" => array('eu27'),
      "countries_selected" => array("eu28"),
      "limited" => FALSE,
      "timeline" => FALSE,
      "time_delay" => 5000,
      "custom_js" => "mainAllYear.js",
      "process_data_function" => "_mmw_charts_get_json_dual_y",
      "chart_options" => array(
        "showvalues" => "0",
        "numberprefix" => "",
        "legendborderalpha" => "50",
        "showborder" => "0",
        "bgcolor" => "FFFFFF,FFFFFF",
        "plotgradientcolor" => " ",
        "showalternatehgridcolor" => "0",
        "showplotborder" => "0",
        "labeldisplay" => "WRAP",
        "divlinecolor" => "CCCCCC",
        "showcanvasborder" => "0",
        "canvasborderalpha" => "0",
        "legendshadow" => "0",
        "linethickness" => "3",
        "xaxisname" => $xaxisname,
        "yaxisname" => $yaxisname,
        "sYAxisName" => "Unit",
        "numberPrefix" => "",
        "sNumberSuffix" => "",
        "bgAlpha" => "100",
        "canvasBgAlpha" => "100",
        "sYAxisMaxValue" => "",
        "theme" => $theme,
        "exportEnabled" => '1',
        "exportAtClientSide" => '1',
      ),
    ),
    "maps/europe2" => array(
      "name" => "Europe map chart",
      "categories_multiselect" => FALSE,
      "type" => "maps/europe2",
      "select_country" => FALSE,
      "countries_selected" => array(),
      "limited" => FALSE,
      "timeline" => TRUE,
      "time_delay" => 2000,
      "custom_js" => "mainMap.js",
      "process_data_function" => "_mmw_charts_get_json_map",
      "chart_options" => array(
        "caption" => "",
        "numberPrefix" => "",
        "theme" => $theme,
        "entityFillColor" => "#cccccc",
        "entityFillHoverColor" => "#0075c2",
        "showLabels" => "0",
        "legendposition" => "BOTTOM",
        "legendborderalpha" => "0",
        "legendbordercolor" => "ffffff",
        "legendallowdrag" => "0",
        "legendshadow" => "0",
        "animation" => "1",
        "showbevel" => "0",
        "usehovercolor" => "1",
        "canvasbordercolor" => "FFFFFF",
        "borderColor" => "#BBBBBB",
        "showlegend" => "1",
        "showshadow" => "0",
        "connectorcolor" => "000000",
        "fillalpha" => "80",
        "hovercolor" => "CCCCCC",
        "hoverOnEmpty" => "0",
        "bgColor" => "#ffffff",
        "bgAlpha" => "100",
        "canvasBgAlpha" => "100",
        "showborder" => 1,
        "exportEnabled" => '1',
        "exportAtClientSide" => '1',
      ),
      "colorrange" => array(
        "minvalue" => 0,
        "startlabel" => "Low",
        "endlabel" => "High",
        "code" => "6BAA01",
        "gradient" => "1",
        "color" => array(
          array(
            "maxvalue" => 0,
            "code" => "e44a00",
          ),
        ),
      ),
    ),
    "scrollstackedcolumn2d" => array(
      "name" => "Scroll stacked column chart",
      "categories_multiselect" => FALSE,
      "type" => "scrollstackedcolumn2d",
      "select_country" => TRUE,
      "countries_exclude" => array('eu27', 'eu28'),
      "limited" => TRUE,
      "countries_selected" => "all",
      "timeline" => FALSE,
      "time_delay" => 5000,
      "custom_js" => "mainAllYearStacked.js",
      "process_data_function" => "_mmw_charts_get_json_all_year",
      "chart_options" => array(
        "xaxisname" => $xaxisname,
        "yaxisname" => $yaxisname,
        "showToolTip" => "1",
        "plotHoverEffect" => "1",
        "showvalues" => "0",
        "numberprefix" => "",
        "legendborderalpha" => "50",
        "showborder" => "0",
        "bgcolor" => "FFFFFF,FFFFFF",
        "plotgradientcolor" => " ",
        "showalternatehgridcolor" => "0",
        "showplotborder" => "0",
        "labeldisplay" => "WRAP",
        "divlinecolor" => "CCCCCC",
        "showcanvasborder" => "0",
        "legendshadow" => "0",
        "bgAlpha" => "100",
        "canvasBgAlpha" => "100",
        "linethickness" => "3",
        "exportEnabled" => '1',
        "exportAtClientSide" => '1',
      ),
    ),
    "stackedarea2d" => array(
      "name" => "Stacked area chart",
      "categories_multiselect" => FALSE,
      "type" => "stackedarea2d",
      "select_country" => TRUE,
      "countries_exclude" => array('eu27', 'eu28'),
      "countries_selected" => "all",
      "limited" => TRUE,
      "timeline" => FALSE,
      "time_delay" => 5000,
      "custom_js" => "mainAllYearStacked.js",
      "process_data_function" => "_mmw_charts_get_json_all_year",
      "chart_options" => array(
        "xaxisname" => $xaxisname,
        "yaxisname" => $yaxisname,
        "showvalues" => "0",
        "numberprefix" => "",
        "legendborderalpha" => "50",
        "borderAlpha" => "20",
        "showCanvasBorder" => "0",
        "usePlotGradientColor" => "0",
        "plotBorderAlpha" => "10",
        "legendShadow" => "0",
        "showborder" => "0",
        "bgcolor" => "FFFFFF,FFFFFF",
        "plotgradientcolor" => " ",
        "showalternatehgridcolor" => "0",
        "showplotborder" => "0",
        "labeldisplay" => "WRAP",
        "divlinecolor" => "CCCCCC",
        "showcanvasborder" => "0",
        "bgAlpha" => "100",
        "canvasBgAlpha" => "100",
        "legendshadow" => "0",
        "linethickness" => "3",
        "exportEnabled" => '1',
        "exportAtClientSide" => '1',
      ),
    ),
  );
  // Create array for return function.
  $static_data = array(
    "countryMapping" => $mapping,
    "colors" => $colors,
    "graphTypes" => $graph_types,
    "nid" => $nid,
  );

  return $static_data;
}
