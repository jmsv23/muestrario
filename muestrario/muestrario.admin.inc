<?php

function muestrario_main()
{
	$output = '<h1>Muestrario main</h1>';
	$output .= implode(',',_generate_catalog_view());
	$output .= '<br /><br /><br />';
	return $output;
}
function _generate_catalog_view()
{
	$cat = array();
	$res = db_query('SELECT * FROM {muestrario} ORDER BY nombre');
	while($row = db_fetch_array($res))
	{
		array_push($cat,$row['nombre']);
	}
return $cat;
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
	$output = '<div id="ajax_catalogo">';
	$output .= _generate_muestrario_item();
	$output .= '</div><div class="muest-float"><p></p><button id="btn-si-item">Si</button><button onClick="noEliminar();">No</button></div>';
	$f = '<div>';
	$f .= drupal_get_form('muestrario_fitem');
	$f .= '</div>';
	return $output.$f;
}

function _generate_muestrario_item()
{
	$output = '<table class="muestrario-cat"><thead><tr><th colspan="6">Items</th></tr>';
	$output .= '<tr><th>ID</th><th>Nombre</th><th>Url</th><th>fid</th><th>Catalogo</th><th>Accion</th></tr></thead>';
	$res = db_query('SELECT muestitem.cid,muestitem.url,muestitem.img,muestitem.nombre,muestrario.nombre AS "catalogo" FROM {muestitem,muestrario} WHERE muestitem.bid = muestrario.bid ORDER BY muestitem.nombre,muestitem.cid');
	while($row = db_fetch_array($res))
	{
			$output .= '<tr><td>'.$row['cid'].'</td><td>'.$row['nombre'].'</td><td>'.$row['url'].'</td><td>'.$row['img'].'</td><td>'.$row['catalogo'].'</td><td><button class="cat-erase" value="'.$row['cid'].'"><span>'.$row['nombre'].'</span></button></td></tr>';
	}
	$output .= '</table>';
	return $output;
}


function muestrario_fitem()
{
	$form['#attributes'] = array('enctype' => "multipart/form-data");
	$form['catalogo'] = array(
		'#type' => 'select',
		'#title' => 'Catalogo',
		'#options' => _get_cat_array(),
		'#description' => 'escoge el catalogo al que pertenece el item',
	);
	$form['nombre'] = array(
		'#type' => 'textfield',
		'#title' => 'nombre',
		'#description' => 'escribe el nombre del atributo',
		'#required' => TRUE,
	);
	$form['url'] = array(
		'#type' => 'textfield',
		'#title' => 'url',
		'#description' => 'url del ejemplo',
		'#required' => TRUE,
	);
	
	$form['imagen'] = array(
		'#type' => 'file',
		'#title' => 'Imagen',
		'#description' => 'sube la imagen del item',
	);
	$form['submit'] = array(
	'#type' => 'submit',
	'#value' => t('agregar')
	);
	return $form;
}

function muestrario_fitem_submit($form, $form_state)
{
	$nombre = $form_state['values']['nombre'];
	$cat = $form_state['values']['catalogo'];
	$url = $form_state['values']['url'];
	
	//manipulacion de archivo
	
	//obtiene, para el usuario actual, las limitaciones en cuanto a:
//extensiones que puede subir, resolución de imágenes y tamaño de archivos
	$limits = _upload_file_limits($user);
	$validators = array(
		/*'file_validate_extensions' => array($limits['extensions']),
		'file_validate_image_resolution' => array($limits['resolution']),
		'file_validate_size' => array($limits['file_size'], $limits['user_size']),*/
	);
	if($file = file_save_upload('imagen', $validators, file_directory_path().'/muestrario',FILE_EXISTS_RENAME))
	{
		$img = $file->fid;
		file_set_status($file, FILE_STATUS_PERMANENT);
		if(db_query("INSERT INTO {muestitem} (bid,url,img,nombre) values(%d,'%s',%d,'%s')",$cat,$url,$img,$nombre) == FALSE)
		{
			$salida = 'ocurrio un error al agregar el registro';
		}
		else
		{
			$salida = 'item agregado con exito';
		}
	}
	else
	{
		$salida = 'error al subir el archivo';
	}
	
	drupal_set_message($salida);
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
	switch($tabla)
	{
		case 'catalogo':
			if(_del_muestrario_item_catalogo($id))
			{
				$output = '<div id="mensaje">registro: '.$id.' eliminado de '.$tabla.'</div>';
				$output .= _generate_muestrario_catalogo();
			}
			else
			{
				$output = '<div id="mensaje">ocurrio un problema al eliminar '.$id.' de '.$tabla.'</div>';
			}
		break;
		case 'item':
			if(_del_muestrario_item_items($id))
			{
				$output = '<div id="mensaje">registro: '.$id.' eliminado de '.$tabla.'</div>';
				$output .= _generate_muestrario_item();
			}
			else
			{
				$output = '<div id="mensaje">ocurrio un problema al eliminar '.$id.' de '.$tabla.'</div>';
			}
		break;
	}
	/*if($tabla == 'catalogo')
	{
		if(_del_muestrario_item_catalogo($id))
		{
			$output = '<div id="mensaje">registro: '.$id.' eliminado de '.$tabla.'</div>';
		}
		else
		{
			$output = '<div id="mensaje">ocurrio un problema al eliminar '.$id.' de '.$tabla.'</div>';
		}
	}*/
	
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
function _del_muestrario_item_items($id)
{
	if(db_query("delete from {muestitem} where cid = ".$id) == FALSE)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}
//funciones auxiliares
function _get_cat_array()
{
	$res = db_query('SELECT * FROM {muestrario} ORDER BY bid');
	while($row = db_fetch_array($res))
	{
		$output[$row['bid']] = $row['nombre'];
	}
	return $output;
}
?>