<?php



/***********************
* response pour le lien ajax defini dans le template
***********************/
function ekomundi_ajax_response() {

	
	$type=func_get_arg(0);  // recoit ajax sinon pb ...
	$div_id =func_get_arg(1);  // le bloc dans lequel les infos sont affiché
	$action = func_get_arg(2); // ce qu'on affiche ..
	
	if ($type == 'ajax') { //=ajax

		switch($action){


			case 'load': // ex: ekomundi_ajax/nojs/[type=ajax]/a_div/load/project
				$content_type = func_get_arg(3);
				
				$output='';
				switch($content_type){
					case 'project':
					case 'ekopoint':
					case 'proposition':
						$attachment_before = '<h4>Sort by</h4><ul>
							<li>'. l(t('all'), 'ekomundi_ajax/ajax/'.$div_id.'/sort/'.$content_type.'/page', array('attributes' => array('class' => array('use-ajax') ) ) ).'</li>
		 					<li>'. l(t('mine'), 'ekomundi_ajax/ajax/'.$div_id.'/sort/'.$content_type.'/page_1', array('attributes' => array('class' => array('use-ajax') ) ) ).'</li>
		 					<li>'. t('my-group').'</li></ul><h4>Result</h4>';
				
						$view = views_get_view($content_type);
						if($view)	{
							$view->attachment_before = $attachment_before;

							$output .= '<div id="view-'.$div_id.'">';
							$output .=  $view->preview('page') ;
							$output .= '</div>';
						}	
						break;

					default:
						$view = views_get_view('ekomundi');
						
						$attachment_before = '<h4>Result</h4>';		

						$view->attachment_before = $attachment_before;
						$view->add_item('block_1', 'filter', 'node', 'type', array('value' => array('value' => $content_type)));
						$output .= '<div id="view-'.$div_id.'">';
						$output .=  $view->preview('page') ;
						$output .= '</div>';
						break;
				}
		





				$commands[] = ajax_command_html('#'.$div_id, $output );
				$page = array('#type' => 'ajax', '#commands' => $commands);
				ajax_deliver($page);				

				break;		
				
			case 'sort':  // ex: ekomundi_ajax/nojs/[type=ajax]/a_div/sort/(project)/block_1
 				$view_name = func_get_arg(3);
				$display_type = func_get_arg(4);				

				$attachment_before = '<h4>Sort by</h4><ul>
					<li>'. l(t('all'), 'ekomundi_ajax/nojs/'.$div_id.'/sort/'.$view_name.'/page', array('attributes' => array('class' => array('use-ajax') ) ) ).'</li>
 					<li>'. l(t('mine'), 'ekomundi_ajax/nojs/'.$div_id.'/sort/'.$view_name.'/page_1', array('attributes' => array('class' => array('use-ajax') ) ) ).'</li>
 					<li>'. t('my-group').'</li></ul><h4>Result</h4>';

				$view = views_get_view($view_name);
				$view->attachment_before = $attachment_before;

				$output  = '<div id="view-'.$div_id.'">';
				$output .=  $view->preview($display_type) ;
				$output .= '</div>';


				$commands[] = ajax_command_html('#view-'.$div_id, $output );
				$page = array('#type' => 'ajax', '#commands' => $commands);
				ajax_deliver($page);				

				break;		
		}
  }
	else{
		//$commands[] = ajax_command_html('#view-'.$div_id, 'Pas de javascript rrr...' );
		//$page = array('#type' => 'ajax', '#commands' => $commands);
		//ajax_deliver($page);	
		return '';
	}

}
/*
function eko_ajax_views_pre_render(&$view) {		
//function _add_attachment_before($view_name,$attachment) {	
	$view = views_get_view($view_name);
	switch($view->name){

		case 'project':
      $view->attachment_before = $attachment;
			$view->save();
		break;

	}
	dpm($view);


}
*/

function ekomundi_autocomplete_response($string) {
  $matches = array();
 
  // Some fantasy DB table which holds cities
  $query = db_select('node', 'n');

  // Select rows that match the string
  $return = $query
    ->fields('n', array('title'))
    ->condition('n.title', '%' . db_like($string) . '%', 'LIKE')
    ->condition('n.type', 'proposition', '=')
    ->range(0, 10)
    ->execute();
 
  // add matches to $matches 
  foreach ($return as $row) {
    $matches[$row->city] = check_plain($row->city);
  }
 
  // return for JS
  drupal_json_output($matches);
}

