<?php

/**
 * @file
 * MMW Charts administration page function file.
 *
 * @brief The page for administration module, choice link for "Glossary",
 *   "Intro video" and image for landing picture.
 *
 * @package mmw_charts
 *
 * @author Makemeweb
 */

/**
 * Callback function for ajax autocomplete field inside administration form.
 *
 * @param string $string
 *   A string, value inside autocomplete field.
 */
function _mmw_charts_admin_page_autocomplete($string) {
  $result = db_query("SELECT title, nid FROM {node} WHERE title LIKE '%$string%' LIMIT 10");
  $matches = array();
  foreach ($result as $item) {
    $matches[$item->title . ' [nid:' . $item->nid . ']'] = check_plain($item->title);
  }
  drupal_json_output($matches);
  exit;
}

/**
 * Form constructor for the administration of mmw_charts module.
 *
 * @return array
 *   An array representing the form definition.
 */
function _mmw_charts_admin_page() {
  $form = array();

  $form['mmw_charts_intro_video_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Intro video link'),
    '#description' => t('Choose the node for the "intro video" link.'),
    '#default_value' => _mmw_charts_get_admin_form_value(variable_get('mmw_charts_intro_video_link', '')),
    '#autocomplete_path' => 'mmw_charts/settings/autocomplete',
  );

  $form['mmw_charts_glossary_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Glossary link'),
    '#description' => t('Choose the node for "the glossary" link.'),
    '#default_value' => _mmw_charts_get_admin_form_value(variable_get('mmw_charts_glossary_link', '')),
    '#autocomplete_path' => 'mmw_charts/settings/autocomplete',
  );

  $form['mmw_charts_landing_image'] = array(
    '#type' => 'managed_file',
    '#title' => t('Landing image'),
    '#description' => t("Choose the picture for the landing image."),
    '#default_value' => variable_get('mmw_charts_landing_image', ''),
    '#upload_location' => 'public://mmw_charts/landing_image/',
    '#upload_validators' => array(
      'file_validate_extensions' => array('png jpg jpeg'),
    ),
  );

  $form['#submit'][] = '_mmw_charts_admin_page_submit';
  return system_settings_form($form);
}

/**
 * Submit function of administation form.
 *
 * @ingroup forms
 */
function _mmw_charts_admin_page_submit($form, &$form_state) {
  // Intro video link.
  _mmw_charts_change_admin_form_value($form_state['values'], 'mmw_charts_intro_video_link');
  // Glossary link.
  _mmw_charts_change_admin_form_value($form_state['values'], 'mmw_charts_glossary_link');
  // Landing image.
  if ($form_state['values']['mmw_charts_landing_image'] != '0') {
    // Load the file via file.fid.
    $file = file_load($form_state['values']['mmw_charts_landing_image']);
    // Change status to permanent.
    $file->status = FILE_STATUS_PERMANENT;
    // Save.
    file_save($file);
    file_usage_add($file, 'file', 'mmw_charts_landing_image', $file->fid);
  }
}

/**
 * Get the correct value for autocomplete callback.
 *
 * @param int $value
 *   The node id of current value for autocomplete field.
 *
 * @return string
 *   Retrun a string, contains the title of node and nid arround bracket.
 */
function _mmw_charts_get_admin_form_value($value) {

  if (!empty($value) && is_numeric($value)) {
    $node = node_load($value);
    if (!empty($node)) {
      return $node->title . ' [nid:' . $node->nid . ']';
    }
  }
  return '';
}

/**
 * Check if value has change, if true, change the real value.
 *
 * @param array $values
 *   An array of values submitted, the $form_state['values'] array.
 * @param string $id
 *   A string, machine name of field.
 */
function _mmw_charts_change_admin_form_value(&$values, $id) {
  if (!empty($values[$id])) {
    preg_match('/\[nid:(.+)\]/', $values[$id], $match);
    $values[$id] = variable_get($id, '');
    if (!empty($match) && count($match) == 2 && !empty($match[1]) && is_numeric($match[1])) {
      $values[$id] = $match[1];
    }
  }
}
