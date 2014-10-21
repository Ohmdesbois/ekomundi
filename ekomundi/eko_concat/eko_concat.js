(function($) {
	$(document).ready(function() {

		$('#hierarchical-select-0-wrapper')	.bind('add-to-dropbox', function(hsid, updateType, settings) {
			var selectedTermTid = $('#' + settings.select_id).val();
			alert('add-to-dropbox :' +selectedTermTid);
		});

		$('#hierarchical-select-0-wrapper').bind('change-hierarchical-select', function(hsid, updateType, settings) {
			var selectedTermTid = $('#' + settings.select_id).val();
			alert('change-hierarchical-select :' +selectedTermTid);
		});
	});


})(jQuery);




