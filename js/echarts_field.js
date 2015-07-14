(function($) {
  Drupal.behaviors.echarts_field = {
    attach: function(context, settings) {
      
      $(document).ready(function(){
          if($('#edit-field-graphic-und-0-graph-type').val() !== 'radar'){
             $('#edit-field-graphic-und-0-max-y').parent().hide();
          }
      });

      $('#edit-field-graphic-und-0-graph-type').change(function(){
          if($(this).val() !== 'radar'){
            $('#edit-field-graphic-und-0-max-y').parent().hide();
          }else{
            $('#edit-field-graphic-und-0-max-y').parent().show();
          }
       });
    }
  };
})(jQuery);
