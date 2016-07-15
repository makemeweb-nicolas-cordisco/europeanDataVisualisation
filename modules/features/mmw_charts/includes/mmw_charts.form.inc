<?php

/**
 * @file
 * MMW Charts node save function file.
 *
 * @brief All functions for the node save values.
 *
 * @package mmw_charts
 *
 * @author Makemeweb
 */

/**
 * Function to convert excel file to json format and save inside node field.
 *
 * @param array $form
 *   Nested array of form elements that comprise the form.
 * @param array $form_state
 *   Keyed array containing the current state of the form.
 */
function _mmw_charts_converts_excel_to_json($form, &$form_state) {

  $values = $form_state['values'];
  if (_mmw_charts_check_change_file($form_state['node'], $values['mmw_charts_excel_file'], $values['mmw_charts_json'])) {

    $file = file_load($values['mmw_charts_excel_file'][LANGUAGE_NONE][0]['fid']);
    $uri = $file->uri;

    $reader = new XLSReader();
    $reader->open(str_replace('public:/', variable_get('file_public_path', 'sites/default/files'), $uri));
    $reader->createJsonFromFile();

    $ouput = $reader->getOutputJson();
    $json = json_encode($ouput);
    $form_state['values']['mmw_charts_json'][LANGUAGE_NONE][0]['value'] = $json;
  }
}

/**
 * Check if file or json value changed with this submit form state.
 *
 * @param object $node
 *   The node object.
 * @param array $file
 *   The file field instance of form_state for node form.
 * @param array $json
 *   The json field instance of form_state for node form.
 *
 * @return bool
 *   Return TRUE if the field is empty or not empty but the value has changed.
 */
function _mmw_charts_check_change_file($node, $file, $json) {

  if ((!empty($file) && !empty($file[LANGUAGE_NONE][0]) && !empty($file[LANGUAGE_NONE][0]['fid'])) &&
      (empty($node->mmw_charts_json) || empty($node->mmw_charts_excel_file) ||
      (!empty($node->mmw_charts_excel_file[LANGUAGE_NONE][0]) && !empty($node->mmw_charts_excel_file[LANGUAGE_NONE][0]['fid']) &&
      $node->mmw_charts_excel_file[LANGUAGE_NONE][0]['fid'] != $file[LANGUAGE_NONE][0]['fid']) ||
      (empty($json) || empty($json[LANGUAGE_NONE][0]) || empty($json[LANGUAGE_NONE][0]['value'])))) {
    return TRUE;
  }
  return FALSE;
}
