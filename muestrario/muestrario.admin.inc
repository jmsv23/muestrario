<?php

function muestrario_main()
{
	$output = '<h1>Muestrario main</h1>';
	return $output;
}

function get_muestrario_catalogo()
{
	$output = '<div id="ajax_catalogo">';
	$output .= _generate_muestrario_catalogo();
	$output .= '</div><div class="muest-float"><p></p><button id="btn-si">Si</button><button onClick="noEliminar();">No</button></div>';
	$f = '<div>';
	$f .= drupal_get_form('muestrario_fcatalogo');
	$f .= '</div>';
	return $output.$f;
}

function _generate_muestrario_catalogo()
{
	$output = '<table class="muestrario-cat"><thead><tr><th colspan="3">Catalogo</th></tr>';
	$output .= '<tr><th>ID</th><th>Nombre</th><th>Accion</th></tr></thead>';
	$res = db_query('SELECT * FROM {muestrario} ORDER BY bid');
	while($row = db_fetch_array($res))
	{
			$output .= '<tr><td>'.$row['bid'].'</td><td>'.$row['nombre'].'</td><td><button class="cat-erase" value="'.$row['bid'].'"><span>'.$row['nombre'].'</span></button></td></tr>';
	}
	$output .= '</table>';
	return $output;
}

function get_muestrario_item()
{
	$output = '<h1>Item</h1>';
	return $output;
}

function muestrario_fcatalogo()
{
	$form['nombre']= array(
		'#type' => 'textfield',
		'#title' => 'nombre',
		'#description' => 'escribe el nombre del catalogo',
		'#required' => TRUE,
	);
	$form['submit'] = array(
	'#type' => 'submit',
	'#value' => t('agregar')
	);
	return $form;
}
function muestrario_fcatalogo_submit($form, $form_state)
{
	$nombre = $form_state['values']['nombre'];
	if($nombre != '')
	{
		if(db_query("INSERT INTO {muestrario} (nombre) values('%s')",$nombre) == FALSE)
		{
			$salida = 'ocurrio un error al agregar el registro';
		}
		else
		{
			$salida = 'catalogo agregado con exito';
		}
	}
	else
	{
		$salida = 'no es un valor valido';
	}
	
	drupal_set_message($salida);
}

//borrado ajax de catalogo
function muestrario_ajax_delete($tabla,$id)
{
	if($tabla == 'catalogo')
	{
		if(_del_muestrario_item_catalogo($id))
		{
			$output = '<div id="mensaje">registro: '.$id.' eliminado de '.$tabla.'</div>';
		}
		else
		{
			$output = '<div id="mensaje">ocurrio un problema al eliminar '.$id.' de '.$tabla.'</div>';
		}
	}
	
	$output .= _generate_muestrario_catalogo();
	drupal_json(array('status' => 0, 'data' => $output));
}

function _del_muestrario_item_catalogo($id)
{
	if(db_query("delete from {muestrario} where bid = ".$id) == FALSE)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}
?>