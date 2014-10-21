<?php

function add_node_id($form, &$form_state,$nid,$h2_title){
		$form['node_'.$nid] = array(
			'#type' => 'checkbox',
			'#title' => '<h2 '.$title_attributes.'>  <a href="'.$node_url.'">'.$title.'</a>  </h2>',
		);
}
