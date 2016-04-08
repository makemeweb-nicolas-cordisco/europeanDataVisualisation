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
    
    
  <style>
    <?php
        foreach($content['staticData']['colors'] as $country_code => $color){
            echo '
            .checkbox-primary.' . $country_code . ' input[type="checkbox"]:checked + label::before, .checkbox-primary.' . $country_code . ' input[type="radio"]:checked + label::before {
                background-color: ' . $color . ';
                border-color: ' . $color . ';
            }';
        }
    ?>
</style>
<div id="main-container" class="charts-container">
    <div class="select-row title-row">
        <h1 class="title" id="content-title">Statistics </h1>
        <button class="btn-print"><i class="fa fa-print"></i>Print</button>
        <button class="btn-print"><i class="fa fa-play"></i>Intro video</button>
    </div>
    <div style="clear: both;"></div>
    
    <div id="container-charts-controls">
        
        <div class="panels">
            <div class="select-row">
                <label for="themes-lvl1" class="select-label">Data:</label>
                <select id="themes-lvl1">
                    <?php
                    $themes_values = explode(",", $content['themes']);
                    $display_type = FALSE;
                    echo '<option class="menu-lvl1" value="" selected>Please choose a data</option>';
                    foreach ($content['menu'] as $item)
                    {
                        $selected = "";
                        foreach($item['children'] as $item2){
                            if(in_array($item2['file'], $themes_values)){
                                $selected = "selected";
                                $display_type = $item['file'];
                            }
                            foreach($item2['children'] as $item3){
                                if(in_array($item3['file'], $themes_values)){
                                    $selected = "selected";
                                    $display_type = $item['file'];
                                }
                            }
                        }

                        if(in_array($item['file'], $themes_values)){
                            $selected = "selected";
                            $display_type = $item['file'];
                        }
                        echo '<option class="menu-lvl1" data-unit="' . $item['unit'] . '" value="' . $item['file'] . '" ' . $selected . '>' . $item['name'] . '</option>';
                    }
                    ?>
                </select>
            </div>
            <div class="select-row">
                <div id="graph-select-error-message" style="display:none;">Graph not compatible</div>
                <label for="chart-types" class="select-label">Graph:</label>
                <select id="chart-types">
                    <?php
                    $selected = "";
                    if(!$content['type']){
                        $selected = "selected";
                    }
                    echo '<option value="" ' . $selected . '>Please choose a graph</option>';
                    foreach ($content['staticData']["graphTypes"] as $typ)
                    {
                        $selected = "";
                        if($content['type'] == $typ['type']){
                            $selected = "selected";
                        }
                        echo '<option data-units-excluded="' . implode(" ", $typ["units_exclude"]) . '" value="' . $typ['type'] . '" ' . $selected . '>' . $typ['name'] . '</option>';
                    }
                    ?>
                </select>
                
            </div>
            <div class="select-row">
                <label for="scenario" class="select-label">Scenario:</label>
                <select id="scenario">
                    <?php foreach($content['otherChartsByYears'] as $key => $other_charts): ?>
                      <option <?php print $key == '0' ? 'value="_none" selected' : 'value="' . $key . '"'; ?>><?php print $other_charts; ?></option>
                    <?php endforeach; ?>
                </select>
            </div>
            <div style="clear: both;"></div>
            <hr>
            <?php
              $categories_checkbox_params = array(
                'menu' => $content['menu'],
                'themes' => $themes_values,
                'display_type' => $display_type,
              );
              print theme('categories_checkbox', $categories_checkbox_params);
            ?>
        </div>
        <?php if($content['type']):?>
            <div style="clear: both;"></div>
            <div id="after-menu">
<!--                <div id="no-data" > 
                    Please choose ...
                    <ul>
                        <li>Data</li>
                        <li>Graph</li>
                        <li>EU Member State(s)</li>
                    </ul> 
                </div>-->
                <div id="countries-chart-container" style="display:none;">
                    <?php if($content['staticData']['graphTypes'][$content['type']]['timeline']): ?>
                        <div id="absolute-year"><?php echo reset($content['yearToDisplay']); ?></div>
                    <?php endif; ?>
                        
                    <div id="chart-container" class="left">
                        <?php if($content['type']): ?>
                            <?php if($content['commission']): ?>
                                <script type="application/json">{
                                    "service" : "charts",
                                    "custom" : "<?php echo $content['baseChartJs']; ?>"
                                }</script>
                            <?php else: ?>
                                <script src="<?php echo $content['baseChartJs']; ?>"></script>
                            <?php endif; ?>
                        <?php endif; ?>
                    </div>
                    <!--countries select div-->
                    <?php if($content['staticData']['graphTypes'][$content['type']]['select_country']){
                        print theme('select_countries', array(
                          'commission' => $content['commission'],
                          'mini_map_js' => $content['miniMapJs'],
                          'colors' => $content['staticData']['colors'],
                          'countries' => $content['countries'],
                          'countries_mapping' => $content['staticData']['countryMapping'],
                          'countries_excludes' => $content['staticData']['graphTypes'][$content['type']]['countries_exclude'],
                          'countries_selected' => $content['staticData']['graphTypes'][$content['type']]['countries_selected'],
                        ));
                    } ?>
                </div>
            </div>
            <div style="clear: both;"></div>

            <!--timeline-->
            <div class="select-row timeline" style="display:none;">
                <?php if($content['staticData']['graphTypes'][$content['type']]['timeline']): ?>
                    <div class="timeline-content">
                        <button id="play"><i class="fa fa-play"></i></button>
                        <div class="slider"></div>
                        <select id="year" style="display:none;">
                            <?php
                            $selected = "selected";
                            foreach ($content['yearToDisplay'] as $year)
                            {
                                echo '<option class="year-item" value="' . $year . '" ' . $selected . '>' . $year . '</option>';
                                $selected = "";
                            }
                            ?>
                        </select>
                    </div>
                <?php endif; ?>
                <div class="exporters">
                    <form method="POST" action="/charts/export">
                        <input type="hidden" name="exporter" class="exporter-value" value=""/>
                        <button id="export-form" type="submit" class="btn btn-submit" style="float: right; margin-right: 250px; margin-top: 20px;"><i class="fa fa-download" style="margin-right: 10px;"></i>Export data (xls)</button>
                        <div style="clear: both;"></div>
                    </form>
                </div>
            </div>
            <div style="clear: both;"></div>
        <?php else: ?>
            <div class="charts-landing-img">
                <img class="img-responsive" src="/<?php print drupal_get_path('module', 'mmw_charts'); ?>/assets/images/dg-ener-reference-scenario-2.jpg">
            </div>
        <?php endif; ?>
    </div>
    <?php
    $data_map = array();
    $countries_mapping = $content['staticData']['countryMapping'];
    $colors = $content['staticData']['colors'];
    foreach ($content['countries'] as $country_key => $country)
    {
        // dataMap used in mini-map.js.
        $data_map[] = array(
          "id" => strtoupper($countries_mapping[$country_key]),
          "value" => 1,
          "color" => $colors[$country_key],
        );
    }
    ?>
    <input type="hidden" class="data-map" value='<?php echo json_encode($data_map); ?>'/>
    <a href="/charts/ajax" style="display: none;" id="ajax-url"></a>
</div>

<?php if(!empty($content['type'])): ?>
    <script>
        var parameters = <?php echo json_encode($content['staticData']['graphTypes'][$content['type']]); ?>;
        var euData = { 
            label: 'Other members of EU',  
            value: '',  
            color: '<?php echo $content['staticData']['colors']['eu28']; ?>',
            isSliced: 1
        };
        var data = null;
        var categories = null;
        var chart = null;
        
        <?php
            $width = 710;
            $height = 500;
            if($content['type'] == "maps/europe2"){
                $width = 940;
                $height = 700;
            }
        ?>
        parameters.width = <?php echo $width; ?>;
        parameters.height = <?php echo $height; ?>;  
        parameters.type = '<?php echo $content['type']; ?>';
        parameters.commission = <?php echo ($content['commission']) ? 1 : 0; ?>;
    </script>
    
<?php endif; ?>

</div>
