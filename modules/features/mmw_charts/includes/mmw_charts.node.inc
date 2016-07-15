<?php

/**
 * @file
 * MMW Charts node function file.
 *
 * @brief All functions for the node render.
 *
 * @package mmw_charts
 *
 * @author Makemeweb
 */

/**
 * Implements hook_node_view().
 *
 * Add custom field for render in template.
 */
function mmw_charts_node_view($node, $view_mode, $langcode) {
  if (!empty($node->mmw_charts_json) && !empty($node->mmw_charts_json) && !empty($node->mmw_charts_json[LANGUAGE_NONE][0]['value'])) {
    _mmw_charts_action($node);
  }
}

/**
 * Load the CSS and JS for charts page.
 *
 * @param string $module_path
 *   Path of this module.
 * @param bool $commission
 *   Load js from commission or from local.
 */
function _mmw_charts_load_assets($module_path, $commission) {

  drupal_add_library('system', 'ui.slider');
  // The CSS filename to load.
  $css_files = array(
    'jquery-ui-slider-pips',
    'font-awesome.min',
    'awesome-bootstrap-checkbox',
    'mmw_charts_main',
  );
  foreach ($css_files as $css_file) {
    drupal_add_css($module_path . '/assets/css/' . $css_file . '.css');
  }
  // The FusionCharts filename to load.
  if ($commission) {
    drupal_add_js('//europa.eu/webtools/load.js', array('defer' => TRUE));
  }
  else {
    $fusioncharts_files = array(
      'fusioncharts',
      'fusioncharts.charts',
      'themes/fusioncharts.theme.fint',
    );
    foreach ($fusioncharts_files as $fusioncharts_file) {
      drupal_add_js($module_path . '/assets/fusioncharts/js/' . $fusioncharts_file . '.js');
    }
  }

  // The JS filename to load.
  $js_files = array(
    'jquery-ui-slider-pips',
    'mmw_charts_main',
    'mmw_charts_export',
  );
  foreach ($js_files as $js_file) {
    drupal_add_js($module_path . '/assets/js/' . $js_file . '.js');
  }
}

/**
 * Prepare the template for charts page.
 *
 * @param object $node
 *   The node object.
 *
 * @see ../templates/node--advanced-fusioncharts.tpl.php
 */
function _mmw_charts_action(&$node) {
  $commission = TRUE;
  $data_json = _mmw_charts_json_to_array($node);
  if (!empty($data_json)) {
    // Initalise variable for Javascript, used in main.js and export.js.
    drupal_add_js(array('mmw_charts' => array('node_id' => $node->nid, 'url_alias' => drupal_get_path_alias('node/' . $node->nid))), 'setting');
    // Get the module path.
    $module_path = drupal_get_path('module', 'mmw_charts');
    // Load CSS and JS.
    _mmw_charts_load_assets($module_path, $commission);
    // Prepare the variables for inject in node object.
    $gets = filter_input_array(INPUT_GET);
    $static_data = _mmw_charts_get_static_data($node->nid);
    $type = ($gets && array_key_exists("type", $gets)) ? $gets['type'] : FALSE;
    $themes = ($gets && array_key_exists("themes", $gets)) ? $gets['themes'] : FALSE;
    $menu = $data_json['sMenu'];
    $countries = $data_json['countries'];
    $year_to_display = $data_json['years'];
    $other_charts_by_years = _mmw_charts_get_nodes_by_data_type($node);

    // Set custom js.
    if (!empty($type)) {
      // Render select countries block.
      $node->content['select_countries'] = _mmw_charts_get_select_countries($static_data, $countries, $type);

      $eu_data = array(
        'label' => 'Other members of EU',
        'value' => '',
        'color' => $static_data['colors']['eu28'],
        'isSliced' => 1,
      );

      $parameters_js = $static_data['graphTypes'][$type];
      $parameters_js['type'] = $type;
      $parameters_js['commission'] = ($commission) ? 1 : 0;
      $parameters_js['width'] = "100%";
      $parameters_js['height'] = 500;
      if ($type == "maps/europe2") {
        $parameters_js['width'] = "100%";
        $parameters_js['height'] = 700;
      }

      drupal_add_js(array('mmw_charts' => array('euData' => $eu_data, 'parameters' => $parameters_js)), 'setting');
      drupal_add_js($module_path . "/assets/js/" . $static_data['graphTypes'][$type]['custom_js']);
      // Check if load mini-map js.
      if ($static_data['graphTypes'][$type]['select_country']) {
        if (!$commission) {
          drupal_add_js($module_path . '/assets/js/mini-map.js');
        }
        else {
          $mini_map_js = $GLOBALS['base_url'] . '/' . $module_path . '/assets/js/mini-map.js';
          $mini_map_script = array(
            '#tag' => 'script',
            '#attributes' => array('type' => "application/json"),
            '#markup' => '{"service" : "charts","custom" : "' . $mini_map_js . '"}',
            '#weight' => 999,
          );
          drupal_add_html_head($mini_map_script, "mmw_charts_mini_map");
        }
      }
      $base_charts_js = $GLOBALS['base_url'] . '/' . $module_path . '/assets/js/baseChart.js';
      if ($commission) {
        $base_charts_js_script = array(
          '#tag' => 'script',
          '#attributes' => array('type' => "application/json"),
          '#markup' => '{"service" : "charts","custom" : "' . $base_charts_js . '"}',
          '#weight' => 1000,
        );
        drupal_add_html_head($base_charts_js_script, "mmw_charts_base_charts_js");
      }
      else {
        drupal_add_js($base_charts_js);
      }
    }

    $data_select_checkboxes = _mmw_charts_generate_data_select($themes, $menu);
    // Create style for.
    $color_style = array();
    foreach ($static_data['colors'] as $country_code => $color) {
      $color_style[] = '.checkbox-primary.' . $country_code . ' input[type="checkbox"]:checked + label::before, .checkbox-primary.' . $country_code . ' input[type="radio"]:checked + label::before {
                background-color: ' . $color . ';
                border-color: ' . $color . ';
            }';
    }
    drupal_add_css(implode(' ', $color_style), 'inline');

    // Get the default landing image.
    $image_background_charts = variable_get('mmw_charts_landing_image', '');
    if (!empty($image_background_charts) && is_numeric($image_background_charts)) {
      $file = file_load($image_background_charts);
      if (!empty($file)) {
        $image_background_charts = file_create_url($file->uri);
      }
    }

    // Set variable in content for used in template.
    $node->content['commission'] = $commission;
    $node->content['staticData'] = $static_data;
    $node->content['intro_video'] = _mmw_charts_get_tooltip_link(variable_get('mmw_charts_intro_video_link', ''), t('Intro video'));
    $node->content['glossary'] = _mmw_charts_get_tooltip_link(variable_get('mmw_charts_glossary_link', ''), t('Glossary'));
    $node->content['type'] = $type;
    $node->content['themes'] = $themes;
    $node->content['yearToDisplay'] = $year_to_display;
    $node->content['yearToDisplay_select'] = _mmw_charts_generate_year_select_for_slider($year_to_display);
    $node->content['menu'] = $menu;
    $node->content['countries'] = $countries;
    $node->content['otherChartsByYears'] = $other_charts_by_years;
    $node->content['data_select'] = $data_select_checkboxes[1];
    $node->content['data_checkboxes'] = $data_select_checkboxes[2];
    $node->content['charts_type_select'] = _mmw_charts_generate_charts_type_select($type, $static_data, $themes);
    $node->content['super_categ'] = $data_select_checkboxes[0];
    $node->content['data_map'] = _mmw_charts_get_data_map($static_data, $countries);
    $node->content['charts_form_exporter'] = drupal_get_form('_mmw_charts_export_form');
    $node->content['image_background_charts'] = $image_background_charts;
  }
}
