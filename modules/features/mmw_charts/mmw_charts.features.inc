<?php
/**
 * @file
 * mmw_charts.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function mmw_charts_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function mmw_charts_node_info() {
  $items = array(
    'advanced_fusioncharts' => array(
      'name' => t('Advanced Fusioncharts'),
      'base' => 'node_content',
      'description' => t('Add Excel file in node for generate chart page'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
