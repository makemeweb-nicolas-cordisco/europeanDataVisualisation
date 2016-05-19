<?php

/**
 * @file
 * Default theme implementation to display a node.
 *
 * Available variables:
 * - $title: the (sanitized) title of the node.
 * - $content: An array of node items. Use render($content) to print them all,
 *   or print a subset such as render($content['field_example']). Use
 *   hide($content['field_example']) to temporarily suppress the printing of a
 *   given element.
 * - $user_picture: The node author's picture from user-picture.tpl.php.
 * - $date: Formatted creation date. Preprocess functions can reformat it by
 *   calling format_date() with the desired parameters on the $created variable.
 * - $name: Themed username of node author output from theme_username().
 * - $node_url: Direct URL of the current node.
 * - $display_submitted: Whether submission information should be displayed.
 * - $submitted: Submission information created from $name and $date during
 *   template_preprocess_node().
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default values can be one or more of the
 *   following:
 *   - node: The current template type; for example, "theming hook".
 *   - node-[type]: The current node type. For example, if the node is a
 *     "Blog entry" it would result in "node-blog". Note that the machine
 *     name will often be in a short form of the human readable label.
 *   - node-teaser: Nodes in teaser form.
 *   - node-preview: Nodes in preview mode.
 *   The following are controlled through the node publishing options.
 *   - node-promoted: Nodes promoted to the front page.
 *   - node-sticky: Nodes ordered above other non-sticky nodes in teaser
 *     listings.
 *   - node-unpublished: Unpublished nodes visible only to administrators.
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 *
 * Other variables:
 * - $node: Full node object. Contains data that may not be safe.
 * - $type: Node type; for example, story, page, blog, etc.
 * - $comment_count: Number of comments attached to the node.
 * - $uid: User ID of the node author.
 * - $created: Time the node was published formatted in Unix timestamp.
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 * - $zebra: Outputs either "even" or "odd". Useful for zebra striping in
 *   teaser listings.
 * - $id: Position of the node. Increments each time it's output.
 *
 * Node status variables:
 * - $view_mode: View mode; for example, "full", "teaser".
 * - $teaser: Flag for the teaser state (shortcut for $view_mode == 'teaser').
 * - $page: Flag for the full page state.
 * - $promote: Flag for front page promotion state.
 * - $sticky: Flags for sticky post setting.
 * - $status: Flag for published status.
 * - $comment: State of comment settings for the node.
 * - $readmore: Flags true if the teaser content of the node cannot hold the
 *   main body content.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 *
 * Field variables: for each field instance attached to the node a corresponding
 * variable is defined; for example, $node->body becomes $body. When needing to
 * access a field's raw values, developers/themers are strongly encouraged to
 * use these variables. Otherwise they will have to explicitly specify the
 * desired field language; for example, $node->body['en'], thus overriding any
 * language negotiation rule that was previously applied.
 *
 * @see template_preprocess()
 * @see template_preprocess_node()
 * @see template_process()
 *
 * @ingroup themeable
 */
?>
<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>

  <?php print render($title_prefix); ?>
  <?php print render($title_suffix); ?>

  <div class="content"<?php print $content_attributes; ?>>
    <?php print render($content['body']); ?>
  </div>

  <div id="main-container" class="charts-container">
    <div class="select-row title-row">
      <h1 class="title" id="content-title"><?php print t('Statistics'); ?></h1>
      <button class="btn-print"><i class="fa fa-print"></i><?php print t('Print'); ?></button>
      <button class="btn-print"><i class="fa fa-play"></i><?php print t('Intro video'); ?></button>
    </div>
    <div class="clearfix"></div>
    
    <div id="container-charts-controls">
      <div class="panels">
        <div class="select-row">
          <label for="themes-lvl1" class="select-label"><?php print t('Data:'); ?></label>
          <?php print $content['data_select']; ?>
        </div>
        <div class="select-row">
          <div id="graph-select-error-message" class="mmw-charts-hidden-element"><?php print t('Graph not compatible'); ?></div>
          <label for="chart-types" class="select-label"><?php print t('Graph:'); ?></label>
          <?php print $content['charts_type_select']; ?>
        </div>
        <div class="select-row">
          <label for="scenario" class="select-label"><?php print t('Scenario:'); ?></label>
          <?php print $content['otherChartsByYears']; ?>
        </div>
        <div class="clearfix"></div>
        <hr>
        <?php print $content['data_checkboxes']; ?>
      </div>
      <?php if($content['type']): ?>
        <div class="clearfix"></div>
        <div id="after-menu">
          <div id="countries-chart-container" class="mmw-charts-hidden-element">
            <?php if($content['staticData']['graphTypes'][$content['type']]['timeline']): ?>
              <div id="absolute-year"><?php echo reset($content['yearToDisplay']); ?></div>
            <?php endif; ?>
            <div id="chart-container" class="left"></div>
            <!--countries select div-->
            <?php if(!empty($content['select_countries'])):
              print $content['select_countries'];
            endif; ?>
          </div>
        </div>
        <div class="clearfix"></div>

        <!--timeline-->
        <div class="select-row timeline" class="mmw-charts-hidden-element">
          <?php if($content['staticData']['graphTypes'][$content['type']]['timeline']): ?>
            <div class="timeline-content">
              <button id="play"><i class="fa fa-play"></i></button>
              <div class="slider"></div>
              <?php print $content['yearToDisplay_select'];  ?>
            </div>
          <?php endif; ?>
          <div class="exporters">
              <?php print drupal_render($content['charts_form_exporter']); ?>
          </div>
        </div>
        <div class="clearfix"></div>
      <?php else: ?>
        <div class="charts-landing-img">
          <?php print theme('image', array(
            'path' => $content['image_background_charts'],
            'attributes' => array(
              'class' => array('img-responsive'),
            ),
          )); ?>
        </div>
      <?php endif; ?>
    </div>
    <?php print $content['data_map']; ?>
    <?php print l('<span>' . t('Charts ajax url') . '</span>', 'mmw_charts/charts/ajax', array(
      'html' => TRUE,
      'attributes' => array(
        'id' => 'ajax-url',
        'class' => array('mmw-charts-hidden-element'),
      ),
    )); ?>
  </div>
</div>
