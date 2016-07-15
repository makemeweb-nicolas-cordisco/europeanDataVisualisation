<?php

/**
 * @file
 * MMW Charts node element function file.
 *
 * @brief All functions for the node element render.
 *
 * @package mmw_charts
 *
 * @author Makemeweb
 */

/**
 * Check if node field json is not empty and return array of json decoded.
 *
 * @param object $node
 *   The node object.
 *
 * @return array
 *   If field is not empty, return an array of json decoded
 *   also not return an empty array.
 */
function _mmw_charts_json_to_array($node) {
  if (!empty($node->mmw_charts_json) &&
      !empty($node->mmw_charts_json[LANGUAGE_NONE]) &&
      !empty($node->mmw_charts_json[LANGUAGE_NONE][0]) &&
      !empty($node->mmw_charts_json[LANGUAGE_NONE][0]['value'])) {
    $json = $node->mmw_charts_json[LANGUAGE_NONE][0]['value'];
    return drupal_json_decode($json);
  }
  return array();
}

/**
 * Get charts nodes by data type.
 *
 * @param object $node
 *   The node object.
 *
 * @return string
 *   An html tags for render date select tag.
 */
function _mmw_charts_get_nodes_by_data_type($node) {

  $year_current_node = field_get_items('node', $node, 'mmw_charts_year');
  $year = array('_none' => $year_current_node[0]['value']);

  $other_nodes = array();
  $query = new entityfieldquery();
  $query->entityCondition('entity_type', 'node')
      ->entityCondition('bundle', 'advanced_fusioncharts')
      ->propertyCondition('status', NODE_PUBLISHED)
      ->propertyCondition('language', $node->language)
      ->propertyCondition('nid', $node->nid, '<>');
  $result = $query->execute();
  if (isset($result['node'])) {
    $other_node_datas = array_keys($result['node']);
    $other_nodes = entity_load('node', $other_node_datas);
  }
  foreach ($other_nodes as $other_node) {
    $year_node = field_get_items('node', $other_node, 'mmw_charts_year');
    if (!empty($year_node)) {
      $year[drupal_get_path_alias('node/' . $other_node->nid)] = $year_node[0]['value'];
    }
  }
  arsort($year);
  if (count($year) > 1) {
    return theme('select', array(
      'element' => array(
        '#attributes' => array(
          'id' => 'scenario',
          'class' => array('labeled'),
        ),
        '#value' => '_none',
        '#options' => $year,
      ),
    ));
  }
  else {
    return '';
  }
}

/**
 * Get html for checkboxes categories, possible multiple level.
 *
 * @param array $themes_values
 *   Data themes selected, to be display on the chart.
 * @param array $menu
 *   Array of element section for data values.
 * @param string $display_type
 *   Charts type display.
 *
 * @return string
 *   Render html theming function, generate checkboxes with depth.
 */
function _mmw_charts_get_categories_checkbox($themes_values, $menu, $display_type) {
  $element = array();
  foreach ($menu as $item) {
    foreach ($item['children'] as $k => $item2) {
      $lvl = 2;
      $hidden = ' mmw-charts-hidden-element';
      $checked = "";
      $item['file'] = (isset($item['file'])) ? $item['file'] : '';
      if (isset($item2['file'])) {
        if (in_array($item2['file'], $themes_values)) {
          $checked = "checked";
        }
        if ($display_type == $item2['file']) {
          $hidden = '';
        }
      }

      $element[$k] = array(
        'parent' => $item['file'],
        'file' => $item2['file'],
        'hidden' => $hidden,
        'no-data' => $item2['no-data'],
        'limited-data' => $item2['limited-data'],
        'unit' => $item2['unit'],
        'name' => $item2['name'],
        'checked' => $checked,
        'description' => $item2['description'],
        'level' => $lvl,
      );
      if (!empty($item2['children'])) {
        _mmw_charts_get_sub_categories_checkbox($element[$k]['children'], $item2['children'], $themes_values, $lvl + 1);
      }
    }
  }
  return theme('mmw_charts_categories_checkbox', array('elements' => $element));
}

/**
 * Get html for checkboxes sub categories.
 *
 * @param array $element
 *   Array of data to be renderer by theming function.
 * @param array $items
 *   Initial array of data.
 * @param array $themes
 *   Data themes selected, to be display on the chart.
 * @param int $lvl
 *   Depth of tree for the element.
 */
function _mmw_charts_get_sub_categories_checkbox(&$element, $items, $themes, $lvl) {

  foreach ($items as $k => $item) {
    if ($lvl == 3) {
      $element[$k] = array('button' => FALSE);
      if (count($item['children']) > 0) {
        $element[$k]['button'] = TRUE;
      }
    }
    if (!$item["no-data"] || !empty($item['children'])) {
      $checked = '';
      if (in_array($item['file'], $themes)) {
        $checked = "checked";
      }
      $element[$k]['no-data'] = $item['no-data'];
      $element[$k]['file'] = $item['file'];
      $element[$k]['name'] = $item['name'];
      $element[$k]['unit'] = $item['unit'];
      $element[$k]['description'] = $item['description'];
      $element[$k]['checked'] = $checked;
      $element[$k]['limited-data'] = $item['limited-data'];
      $element[$k]['level'] = $lvl;
      if (!empty($item['children'])) {
        _mmw_charts_get_sub_categories_checkbox($element[$k]['children'], $item['children'], $themes, $lvl + 1);
      }
    }
  }
}

/**
 * Generate template for select countries list block.
 *
 * @param array $static_data
 *   Static data defined by chart type to configure chart
 *   display and data generation.
 * @param array $countries
 *   Countries available in xls data file.
 * @param string $type
 *   Charts type display.
 *
 * @return array
 *   An array of two data for countries values.
 */
function _mmw_charts_get_select_countries($static_data, $countries, $type) {

  $checked = '';
  $countries_data = array();
  foreach ($countries as $country_key => $country) {
    if ($country_key != 'eu27' || !in_array($country_key, $static_data['graphTypes'][$type]['countries_exclude'])) {
      if ($static_data['graphTypes'][$type]['countries_selected'] == "all") {
        // All but not eu28.
        if ($country_key != "eu28") {
          $checked = 'checked';
        }
      }
      else {
        $checked = '';
        if (in_array($country_key, $static_data['graphTypes'][$type]['countries_selected'])) {
          $checked = 'checked';
        }
      }
      $countries_data[] = array(
        'key' => $country_key,
        'country' => $country,
        'mapping' => $static_data['countryMapping'][$country_key],
        'checked' => $checked,
        'color' => $static_data['colors'][$country_key],
      );
    }
  }

  return theme('select_countries', array(
    'countries_data' => $countries_data,
    'countries_selected' => $static_data['graphTypes'][$type]['countries_selected'],
    'countries_exclude' => $static_data['graphTypes'][$type]['countries_exclude'],
  ));
}

/**
 * Generate a html select tag for data of charts.
 *
 * @param string $themes
 *   Data themes selected, to be display on the chart.
 * @param array $menu
 *   Array of element section for data values.
 *
 * @return array
 *   Array of render html theming function, generate select and checkbox tag.
 */
function _mmw_charts_generate_data_select($themes, $menu) {

  $data_select_element = array(
    '#options' => array(),
    '#value' => 0,
    '#attributes' => array('id' => 'themes-lvl1'),
  );

  $themes_values = explode(',', $themes);
  $display_type = FALSE;
  $output_select_level0 = '';
  $output_select = '';
  foreach ($menu as $item) {
    $selected = "";
    $lvl1_hidden = 'hidden';
    $item['file'] = (isset($item['file'])) ? $item['file'] : "empty";
    $item['name'] = (isset($item['name'])) ? $item['name'] : "";

    // First foreach to know item selected in subset for hidden property.
    foreach ($item['children'] as $item2) {
      $lvl1_selected = "";
      if (in_array($item2['file'], $themes_values)) {
        $lvl1_hidden = "";
      }
      foreach ($item2['children'] as $item3) {
        if (in_array($item3['file'], $themes_values)) {
          $lvl1_hidden = "";
        }
        foreach ($item3['children'] as $item4) {
          if (in_array($item4['file'], $themes_values)) {
            $lvl1_hidden = "";
          }
        }
      }
    }
    // Second foreach.
    foreach ($item['children'] as $item2) {
      $lvl1_selected = "";
      if (in_array($item2['file'], $themes_values)) {
        $selected = "selected";
        $lvl1_selected = "selected";
        $display_type = $item2['file'];
      }
      foreach ($item2['children'] as $item3) {
        if (in_array($item3['file'], $themes_values)) {
          $selected = "selected";
          $lvl1_selected = "selected";
          $display_type = $item2['file'];
        }
        foreach ($item3['children'] as $item4) {
          if (in_array($item4['file'], $themes_values)) {
            $selected = "selected";
            $lvl1_selected = "selected";
            $display_type = $item2['file'];
          }
        }
      }
      $output_select .= '<option class="menu-lvl1 ' . $item['file'] . ' ' . $lvl1_hidden . '" data-description="' . $item2['description'] . '" data-limited="' . $item2['limited-data'] . '" value="' . $item2['file'] . '" ' . $lvl1_selected . '>' . $item2['name'] . '</option>';
    }

    $data_select_element['options'][$item['file']] = array(
      'value' => $item['name'],
    );
    $output_select_level0 .= '<option class="menu-lvl0" value="' . $item['file'] . '" data-description="' . $item['description'] . '" ' . $selected . '>' . $item['name'] . '</option>';
  }
  if (empty($themes)) {
    $selected = "selected";
    $disabled = "disabled";
  }
  else {
    $selected = '';
    $disabled = '';
  }
  $output_select_level0 = '<select id="themes-lvl0" class="labeled"><option class="menu-lvl0" value="" ' . $selected . '>' . t('Please choose a category') . '</option>' . $output_select_level0 . '</select>';
  $output_select = '<select id="themes-lvl1" ' . $disabled . '><option class="menu-lvl1" value="" ' . $selected . '>' . t('Please choose an indicator') . '</option>' . $output_select . '</select>';

  return array(
    $output_select_level0,
    $output_select,
    _mmw_charts_get_categories_checkbox($themes_values, $menu, $display_type),
  );
}

/**
 * Generate select tag for year slider bar.
 *
 * @param array $year_to_display
 *   Pair of url and year number.
 *
 * @return string
 *   An html select tag, used for create slide bar years.
 */
function _mmw_charts_generate_year_select_for_slider($year_to_display) {
  $output_select = '<select id="year" class="mmw-charts-hidden-element visible-xs-inline-block hidden-sm hidden-md hidden-lg">';
  $selected = "selected";
  foreach ($year_to_display as $year) {
    $output_select .= '<option class="year-item" value="' . $year . '"' . $selected . '>' . $year . '</option>';
    $selected = "";
  }
  return $output_select . '</select>';
}

/**
 * Generate charts type for select tag.
 *
 * @param string $type
 *   Charts type display.
 * @param array $static_data
 *   Static data defined by chart type to configure chart
 *   display and data generation.
 *
 * @return string $output_select
 *   An html select tag for charts type values.
 */
function _mmw_charts_generate_charts_type_select($type, $static_data, $themes) {
  if (empty($themes)) {
    $disabled = 'disabled';
  }
  else {
    $disabled = '';
  }
  $output_select = '<select id="chart-types" class="labeled" ' . $disabled . '>';
  $selected = "";
  if (!$type) {
    $selected = "selected";
  }
  $output_select .= '<option value="" ' . $selected . '>' . t('Please choose a graph') . '</option>';
  foreach ($static_data["graphTypes"] as $typ) {
    $selected = "";
    if ($type == $typ['type']) {
      $selected = "selected";
    }
    $output_select .= '<option data-limited="' . $typ["limited"] . '" value="' . $typ['type'] . '" ' . $selected . '>' . $typ['name'] . '</option>';
  }
  return $output_select . '</select>';
}

/**
 * Generate super categories for select tag.
 *
 * @param string $type
 *   Charts type display.
 * @param array $static_data
 *   Static data defined by chart type to configure chart
 *   display and data generation.
 *
 * @return string $output_select
 *   An html select tag for super categ values.
 */
function _mmw_charts_generate_super_categ($type, $static_data) {
  $static_data["superCateg"] = array(
    t('Security of supply'),
    t('Electricity'),
    t('Transport'),
    t('Energy efficiency'),
    t('Decarbonisation'),
    t('Markets and competitiveness'),
  );
  $output_select = '<select id="super-categ" class="labeled">';
  $selected = "selected";
  $output_select .= '<option value="" ' . $selected . '>' . t('Please choose a category') . '</option>';
  foreach ($static_data["superCateg"] as $categ_value => $categ_name) {
    $selected = "";
    $output_select .= '<option value="' . $categ_value . '" ' . $selected . '>' . $categ_name . '</option>';
  }
  return $output_select . '</select>';
}

/**
 * Get data value for the map in fusioncharts.
 *
 * @param array $static_data
 *   Static data defined by chart type to configure chart
 *   display and data generation.
 * @param array $countries
 *   Countries available in xls data file.
 *
 * @return string
 *   An html render by theming with hidden theme.
 */
function _mmw_charts_get_data_map($static_data, $countries) {
  $data_map = array();
  $countries_mapping = $static_data['countryMapping'];
  $colors = $static_data['colors'];
  foreach ($countries as $country_key => $country) {
    // Data_map used in mini-map.js.
    $data_map[] = array(
      "id" => strtoupper($countries_mapping[$country_key]),
      "value" => 1,
      "color" => $colors[$country_key],
    );
  }
  return theme('hidden', array('element' => array('#value' => json_encode($data_map), '#attributes' => array('class' => 'data-map'))));
}

/**
 * Form constructor for the export charts form.
 *
 * @ingroup forms
 */
function _mmw_charts_export_form($form, &$form_state) {
  $form = array(
    '#method' => 'post',
    '#action' => url('mmw_charts/charts/export'),
    '#suffix' => '<div class="clearfix"></div>',
    'exporter' => array(
      '#type' => 'hidden',
      '#value' => '',
      '#attributes' => array('class' => array('exporter-value')),
    ),
    'export-form' => array(
      '#markup' => '<button id="export-form" type="submit" class="btn btn-submit">
                  <i class="fa fa-download"></i><span>' . t('Export data (xls)') . '</span></button>',

    ),
  );
  return $form;
}

/**
 * Get a link for node reference for an tooltip link.
 *
 * @param int $value
 *   The nid of node for the link.
 *
 * @return string
 *   An html render for a link.
 */
function _mmw_charts_get_tooltip_link($value, $text) {
  if (!empty($value) && is_numeric($value)) {
    $node = node_load($value);
    if (!empty($node)) {
      return "<span class='btn-print'> | </span>" . l($text, 'node/' . $node->nid);
    }

  }
  return '';
}
