<?php

/**
 * @file
 * Default theme functions.
 */

/**
 * Implements theme_preprocess().
 */
function energy_preprocess_node(&$variables) {
  global $base_url;
  $output = '';
  $display_user_picture = TRUE;
  $prefixe = '';
  $suffixe = '';
  // Get node type.
  $node = $variables['node'];
  $type = $node->type;
  $content = $variables['content'];

  switch ($type) {
    case 'links':
      $fields = array(
        'picture' => array(),
        'body'    => array(),
        'hidden'  => array(
          'comments',
          'links',
          'print_links',
          'og_group_ref',
          'language'),
        'group' => array('group_audience', 'group_content_access'),
      );
      $display_user_picture = FALSE;
      break;

    case 'news':
      $fields = array(
        'picture' => array('field_news_picture'),
        'body'    => array('body'),
        'hidden'    => array(
          'comments',
          'links',
          'print_links',
          'og_group_ref',
          'language'),
        'group'   => array('group_audience', 'group_content_access'),
      );
      $display_user_picture = FALSE;
      break;

    case 'community':
      $fields = array(
        'picture' => array('field_thumbnail'),
        'body'  => array('body'),
        'hidden'  => array(
          'comments',
          'links',
          'print_links',
          'field_thumbnail',
          'og_roles_permissions',
          'og_group_ref',
          'language'),
        'group' => array('group_group', 'group_access'),
      );
      $display_submitted = FALSE;
      break;

    case 'blog_post':
      $fields = array(
        'picture' => array(),
        'body'  => array('body'),
        'hidden'  => array(
          'comments',
          'links',
          'print_links',
          'og_group_ref',
          'language'),
        'group' => array('group_audience', 'group_content_access'),
      );
      break;

    case 'idea':
      $fields = array(
        'picture' => array(),
        'body'  => array('body'),
        'hidden'  => array('comments',
          'links',
          'field_watching',
          'print_links',
          'og_group_ref',
          'language'),
        'group' => array('group_audience', 'group_content_access'),
      );
      $display_submitted = FALSE;
      if ($content['field_watching']['#object']->field_watching['und'][0]['value']) {
        $suffixe .= '<div class="no_label">';
        $suffixe .= '<span class="label label-success t_upper f_right"><span class="glyphicon glyphicon-eye-open"></span>' . t('watched') . '</span>';
        $suffixe .= '</div>';
      }
      break;

    case 'webform':
      $fields = array(
        'picture' => array(),
        'body'  => array('body'),
        'hidden'  => array('comments',
          'links',
          'print_links',
          'og_group_ref',
          'language'),
        'group' => array('group_audience', 'group_content_access'),
      );
      $display_user_picture = FALSE;
      break;

    case 'gallerymedia':
      $fields = array(
        'picture' => array(),
        'body'  => array('body'),
        'hidden'  => array(
          'field_video_upload',
          'field_picture_upload',
          'comments',
          'links',
          'print_links',
          'og_group_ref',
          'language'),
        'group' => array('group_audience', 'group_content_access'),
      );
      $suffixe = $gallerymedia_items;
      break;

    case 'page':
      $fields = array(
        'picture' => array(),
        'body'  => array('body'),
        'hidden'  => array(
          'field_picture_upload',
          'comments',
          'links',
          'print_links',
          'og_group_ref',
          'language'),
        'group' => array('group_audience', 'group_content_access'),
      );
      break;

    case 'article':
      $fields = array(
        'picture' => array(),
        'body'  => array('body'),
        'hidden'  => array(
          'comments',
          'links',
          'print_links',
          'field_article_publication_date',
          'og_group_ref',
          'language'),
        'group' => array('group_audience', 'group_content_access'),
      );
      break;

    default:
      $fields = array(
        'picture' => array(),
        'body'  => array('body'),
        'hidden'  => array(
          'comments',
          'links',
          'print_links',
          'og_group_ref',
          'language'),
        'group' => array('group_audience', 'group_content_access'),
      );
      break;
  }

  // Check if this content is private.
  if (isset($fields['group'])) {
    foreach ($fields['group'] as $id) {
      if (isset($content[$id]['#items'][0]['value']) && $content[$id]['#items'][0]['value'] == 2) {
        $output .= '<div class="node-private label label-default clearfix">';
        $output .= '<span class="glyphicon glyphicon-lock"></span>';
        $output .= t('This content is private');
        $output .= '</div>';
        break;
      }
    }
  }

  // Display non hidden fields.
  $display_content = FALSE;
  foreach ($variables['content'] as $key => $value) {
    if (!in_array($key, $fields['hidden']) && !in_array($key, $fields['group'])) {
      $display_content = TRUE;
      if (isset($value['#weight'])) {
        $fields['content'][$key] = $value['#weight'];
      }
    }
  }

  if ($display_content && !empty($fields['content'])) {
    // Sort fields by weight.
    asort($fields['content']);

    foreach ($fields['content'] as $key => $value) {
      $field = '';

      if (isset($variables['content'][$key]['#label_display'])) {
        // Check if it is the first field.
        $first = FALSE;
        reset($fields['content']);
        if ($key === key($fields['content'])) {
          $first = TRUE;
        }
        // Check if it is the last field.
        $last = FALSE;
        end($fields['content']);
        if ($key === key($fields['content'])) {
          $last = TRUE;
        }

        $field .= '<div class="row c_left field field-' . $variables['content'][$key]['#field_name'] . ($first ? " first" : "") . ($last ? " " : "") . '">';

        switch ($variables['content'][$key]['#label_display']) {
          case 'hidden':
            if ($variables['content'][$key]["#field_name"] == "field_more_event_information" || $variables['content'][$key]["#field_name"] == "field_topic_legislative_bck" || $variables['content'][$key]["#field_name"] == "field_consultation_period" || $variables['content'][$key]["#field_name"] == "field_policy_field_s_" || $variables['content'][$key]["#field_name"] == "field_target_group_s_" || $variables['content'][$key]["#field_name"] == "field_objective_of_the_consultat" || $variables['content'][$key]["#field_name"] == "field_how_to_submit_your_contrib" || $variables['content'][$key]["#field_name"] == "field_contact" || $variables['content'][$key]["#field_name"] == "field_number_of_responses_receiv" || $variables['content'][$key]["#field_name"] == "field_legislative_background" || $variables['content'][$key]["#field_name"] == "field_more_information_box"  || $variables['content'][$key]["#field_name"] == "field_more_information" || $variables['content'][$key]["#field_name"] == "field_topic_links") {
              $field .= '<div class="col-lg-12 col-md-12 col-sm-12 col-xs-12"><div class="group-more-info field-group-div" id="node-events-full-group-more-info"><h2><span>' . $variables['content'][$key]["#title"] . '</span></h2>' . render($variables['content'][$key]) . '</div></div>';
            }
            else {
              $field .= '<div class="col-lg-12 col-md-12 col-sm-12 col-xs-12">' . render($variables['content'][$key]) . '</div>';
            }
            break;

          case 'above':
            if (isset($variables['content'][$key]['#title'])) {
              $variables['content'][$key]['#label_display'] = 'hidden';
              $field .= '<div class="col-lg-12 col-md-12 col-sm-12 col-xs-12 field-label">' . $variables['content'][$key]['#title'] . '</div></div>';
              $field .= '<div class="row"><div class="col-lg-12 col-md-12 col-sm-12 col-xs-12">' . render($variables['content'][$key]) . '</div>';
            }
            else {
              $field .= '<div class="col-lg-12 col-md-12 col-sm-12 col-xs-12">' . render($variables['content'][$key]) . '</div>';
            }
            break;

          case 'inline':
          default:
            if (isset($variables['content'][$key]['#title'])) {
              $variables['content'][$key]['#label_display'] = 'hidden';
              $field .= '<div class="col-lg-2 col-md-3 col-sm-3 col-xs-4 field-label">' . $variables['content'][$key]['#title'] . '</div>';
              $field .= '<div class="col-lg-10 col-md-9 col-sm-9 col-xs-8">' . render($variables['content'][$key]) . '</div>';
            }
            else {
              $field .= '<div class="col-lg-12 col-md-12 col-sm-12 col-xs-12">' . render($variables['content'][$key]) . '</div>';
            }
            break;
        }

        $field .= '</div>';
      }
      else {
        $field .= '<div class="row"><div class="col-lg-12 col-md-12 col-sm-12 col-xs-12">' . render($variables['content'][$key]) . '</div></div>';
      }

      if (isset($variables['content'][$key]['#field_name']) && in_array($variables['content'][$key]['#field_name'], $fields['picture'])) {
        $output .= '<div class="no_label center">' . $field . '</div>';
      }
      else {
        $output .= $field;
      }
    }
  }

  // Display information about node (group, date and author).
  $base_theme = multisite_drupal_toolbox_get_base_theme();
  // $base_theme = "ec_resp";
  $display_workbench = call_user_func_array($base_theme . '_block_render', array('workbench', 'block'));

  if (isset($display_submitted) || $display_workbench) {
    $output .= '<div class="row node-info">';
    if ($display_workbench) {
      $output .= '<div class="node-info-workbench col-lg-6 col-md-6 col-sm-6 col-xs-12">';
      $output .= '<div class="well well-sm node-workbench"><small>';
      $output .= $display_workbench;
      $output .= '</small></div>';
      $output .= '</div>';
    }

    if ($display_submitted) {
      $output .= '<div class="node-info-submitted col-lg-6 col-md-6 col-sm-6 col-xs-12' . ($display_workbench ? "" : " col-lg-offset-6 col-md-offset-6 col-sm-offset-6") . '">';
      $output .= '<div class="well well-sm node-submitted clearfix"><small>';
      // Author picture.
      if ($display_user_picture) {
        $output .= $user_picture;
      }
      // Publication date.
      $output .= $submitted;
      $output .= '</small></div>';
      $output .= '</div>';
    }
    $output .= '</div>';
  }

  $images_location = $base_url . '/' . drupal_get_path('theme', 'energy');

  if (isset($output)) {
    $reg_exurl_pdf = "/<a.*?href=[\"']?(.*?)\.pdf[\"']?.*?>(.*?)<\\/a>/i";
    $reg_exurl_doc = "/<a.*?href=[\"']?(.*?)\.doc[\"']?.*?>(.*?)<\\/a>/i";
    $reg_exurl_zip = "/<a.*?href=[\"']?(.*?)\.zip[\"']?.*?>(.*?)<\\/a>/i";
    $reg_exurl_xls = "/<a.*?href=[\"']?(.*?)\.xls[\"']?.*?>(.*?)<\\/a>/i";

    if (preg_match_all($reg_exurl_pdf, $output, $url_pdf)) {
      $output = preg_replace($reg_exurl_pdf, '$0' . ' <img src="' . $images_location . '/images/pdf_icon.png" />', $output);
    }
    if (preg_match_all($reg_exurl_zip, $output, $url_zip)) {
      $output = preg_replace($reg_exurl_zip, '$0' . ' <img src="' . $images_location . '/images/archive_icon.png" />', $output);
    }
    if (preg_match_all($reg_exurl_doc, $output, $url_doc)) {
      $output = preg_replace($reg_exurl_doc, '$0' . ' <img src="' . $images_location . '/images/word_icon.gif" />', $output);
    }
    if (preg_match_all($reg_exurl_xls, $output, $url_xls)) {
      $output = preg_replace($reg_exurl_xls, '$0' . ' <img src="' . $images_location . '/images/excel_icon.gif" />', $output);
    }
  }
  $variables['content_with_icons'] = $output;
}
