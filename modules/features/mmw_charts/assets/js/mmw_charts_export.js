/**
 * @file
 */

jQuery(document).ready(function ($) {
    $('#export-form').click(function (e) {
        e.preventDefault();
        var data = {};
        var countries = [];
        $('#country-select').find('input').each(function () {
          if ($(this).prop('checked')) {
            countries.push($(this).val());
          }
        });
        data.countries = countries;
        if ($('#themes').length > 0) {
            data.themes = $('#themes input:checkbox:checked').map(function () {
                return this.value;
            }).get();
        }
        else {
            data.themes = [$('#themes-lvl1').val()];
        }

        data.charttype = $('#chart-types').val();
        data.nid = Drupal.settings.mmw_charts.node_id;

        $('.exporter-value').val(JSON.stringify(data));
        $(this).closest('form').submit();
    });
});
