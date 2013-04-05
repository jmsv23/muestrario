<?php

function muestrario_main()
{
	$output = '<h1>Muestrario main</h1>';
	return $output;
}

function get_muestrario_catalogo()
{
	$output = '<table class="muestrario-cat"><thead><tr><th colspan="3">Catalogo</th></thead>';
	$res = db_query('SELECT * FROM {muestrario} ORDER BY bid');
	while($row = db_fetch_array($res))
	{
			$output .= '<tr><td>'.$row['bid'].'</td><td>'.$row['nombre'].'</td><td><a href="javascript:alert(\''.$row['bid'].'\');">eliminar</a></td></tr>';
	}
	$output .= '</table>';
	$f = '<div>';
	$f .= drupal_get_form('muestrario_fcatalogo');
	$f .= '</div>';
	return $output.$f;
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
			$salida = 'catalogo agregado con exito';
		}
		else
		{
			$salida = 'ocurrio un error al agregar el registro';
		}
	}
	else
	{
		$salida = 'no es un valor valido';
	}
	
	drupal_set_message($salida);
}

?>