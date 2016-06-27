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

  <div class="mmw-charts-main-container">
    <div class="title-row tooltips-row row hidden-xs">
      <div class="col-xs-12">
        <button class="btn-print"><i class="fa fa-print"></i><?php print t('Print'); ?></button>
        <?php print $content['intro_video']; ?>
        <?php print $content['glossary']; ?>
      </div>
    </div>
    
    <div id="container-charts-controls" class="row">
        <div class="col-xs-12">
            <div class="panels col-xs-12">
                <?php if (!empty($content['otherChartsByYears'])): ?>
                  <div class="row">
                      <div class="col-xs-12">
                          <div class="select-row select-scenario pull-right pull-left-sm">
                              <label for="scenario" class="select-label"><?php print t('Scenario:'); ?></label>
                              <?php print $content['otherChartsByYears']; ?>
                          </div>
                      </div>
                  </div>
                <?php endif; ?>

                <div class="row">
                    <div class="col-xs-12">
                    <div class="select-row keep-float">
                        <label for="super-categ" class="select-label"><?php print t('Data:'); ?></label>
                        <?php print $content['super_categ']; ?>
                    </div>
                    <div class="select-row keep-float">
                        <?php print $content['data_select']; ?>
                    </div>
                    <div class="select-row pull-right pull-left-sm">
                        <div id="graph-select-error-message" class="mmw-charts-hidden-element"><?php print t('Graph not compatible'); ?></div>
                        <label for="chart-types" class="select-label"><?php print t('Graph:'); ?></label>
                        <?php print $content['charts_type_select']; ?>
                    </div>
                        </div>
                </div>

                <hr>

                <div class="row">
                    <div class="col-xs-12">
                      <?php print $content['data_checkboxes']; ?>
                    </div>
                </div>
            </div>
        </div>
      <div class="charts-landing-img col-xs-12" <?php if(!empty($content['themes'])): ?>style="display:none;"<?php endif; ?>>
        <?php if(!empty($content['image_background_charts'])):
          print theme('image', array(
            'path' => $content['image_background_charts'],
            'attributes' => array(
              'class' => array('img-responsive'),
            ),
          ));
        endif; ?>
      </div>
      <?php if($content['type']): ?>
        <div id="after-menu" class="col-xs-12">
          <div id="countries-chart-container" class="mmw-charts-hidden-element">
            <?php if($content['staticData']['graphTypes'][$content['type']]['timeline']): ?>
              <div id="absolute-year" class="countdown-year hidden-xs"><?php echo reset($content['yearToDisplay']); ?></div>
            <?php endif; ?>
            <div class="row">
              <?php if(!empty($content['staticData']['graphTypes'][$content['type']]['select_country'])): ?>
                <div id="chart-container" class="col-sm-9"></div>
                <!--countries select div-->
                <?php print $content['select_countries']; ?>
              <?php else: ?>
                <div id="chart-container" class="col-xs-12"></div>
              <?php endif; ?>
            </div>
          </div>
        </div>
        <div class="clearfix"></div>
        <!--timeline-->
        <div class="col-xs-12">
          <div class="timeline" class="mmw-charts-hidden-element">
            <?php if($content['staticData']['graphTypes'][$content['type']]['timeline']): ?>
              <div class="timeline-content">
                <button id="play"><i class="fa fa-play"></i></button>
                <div class="slider hidden-xs"></div>
                <?php print $content['yearToDisplay_select'];  ?>
              </div>
              <div class="exporters player-button">
            <?php else: ?>
              <div class="exporters">
            <?php endif; ?>
                <a href="#" id="show-country-select-mmw-charts" class="visible-xs-inline-block hidden-sm hidden-md hidden-lg">
                    <i class="fa fa-filter" aria-hidden="true"></i>
                    <span><?php print t('Show countries'); ?></span>
                </a>
                <?php print drupal_render($content['charts_form_exporter']); ?>
            </div>
          </div>
        </div>
      <?php endif; ?>
    </div>
    <div class="tooltips-row row visible-xs-block hidden-sm hidden-md hidden-lg">
      <div class="col-xs-12">
        <button class="btn-print"><i class="fa fa-print"></i><?php print t('Print'); ?></button>
        <?php print $content['intro_video']; ?>
        <?php print $content['glossary']; ?>
      </div>
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
