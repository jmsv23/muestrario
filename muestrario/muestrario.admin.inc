<?php

function muestrario_main()
{
	$output = '<h1>Muestrario</h1>';
	$output .= generate_catalog_view();
	return $output;
}
function generate_catalog_view()
{
	$output = '';
	$res = db_query('SELECT * FROM {muestrario} ORDER BY nombre');
	while($row = db_fetch_array($res))
	{
		$output .= '<div class="muest-catalog">';
		$output .= '<h3>'.$row['nombre'].'</h3>';
		$output .='<div>';
		$output .= generate_muestrario_item_view($row['bid']);
		$output .= '</div>';
		$output .= '</div>';
	}
return $output;
}
//generamos el html del item
function generate_muestrario_item_view($cat)
{
	$output = '';
	$presetname = 'muestrario_miniatura';
	$res = db_query('SELECT * FROM {muestitem} WHERE bid = '.$cat);
	$flag = 0;
	while($row = db_fetch_array($res))
	{
		//determinamos si es el segundo elemento para agregar la clase item-middle
		if($flag == 1)
		{
			$intclass = ' item-middle';	
		}
		else
		{
			$intclass = '';
		}
		
		
		$output .= '<div class="muest-item'.$intclass.'">';
		//generamos la url de la imagen dependiente de imagecache
		$output .= '<a href="'.$row['url'].'" class="m-img" target="_blank"><img src="'.base_path().imagecache_create_path($presetname,_get_muestrario_filepath($row['cid'])).'" /></a>';
		$output .= '<a href="'.$row['url'].'" class="m-link" target="_blank">ver '.$row['nombre'].'</a>';
		$output .= '</div>';
		
		
		//establecemos el contador para identificar el elemento del centro
		
		if($flag >= 2)
		{
			$flag = 0;
		}
		else
		{
			$flag++;
		}
	}
	return $output;
}
//genera la pagina de catalogo /store/muestrario/catalgo
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
//genera la tabla de catalogo
function _generate_muestrario_catalogo()
{
	$output = '<table class="muestrario-cat"><thead><tr><th colspan="5">Catalogo</th></tr>';
	$output .= '<tr><th>ID</th><th>Nombre</th><th>Descripcion</th><th>ID de atributo</th><th>Accion</th></tr></thead>';
	$res = db_query('SELECT * FROM {muestrario} ORDER BY bid');
	while($row = db_fetch_array($res))
	{
			$output .= '<tr><td>'.$row['bid'].'</td><td>'.$row['nombre'].'</td><td>'.$row['descripcion'].'</td><td>'.$row['aid'].'</td><td><button class="cat-erase" value="'.$row['bid'].'"><span>'.$row['nombre'].'</span></button></td></tr>';
	}
	$output .= '</table>';
	return $output;
}
//genera la pagina de item /store/muestrario/item
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
	$output = '<table class="muestrario-cat"><thead><tr><th colspan="7">Items</th></tr>';
	$output .= '<tr><th>ID</th><th>Nombre</th><th>Url</th><th>fid</th><th>Catalogo</th><th>oid</th><th>Accion</th></tr></thead>';
	$res = db_query('SELECT muestitem.cid,muestitem.url,muestitem.img,muestitem.nombre,muestitem.oid,muestrario.nombre AS "catalogo" FROM {muestitem,muestrario} WHERE muestitem.bid = muestrario.bid ORDER BY muestitem.nombre,muestitem.cid');
	while($row = db_fetch_array($res))
	{
			$output .= '<tr><td>'.$row['cid'].'</td><td>'.$row['nombre'].'</td><td>'.$row['url'].'</td><td>'.$row['img'].'</td><td>'.$row['catalogo'].'</td><td>'.$row['oid'].'</td><td><button class="cat-erase" value="'.$row['cid'].'"><span>'.$row['nombre'].'</span></button></td></tr>';
	}
	$output .= '</table>';
	return $output;
}

//funcion que genera el formulario de ingreso de items
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
//procesa el formulario de items
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
	);
	$aid = _get_id_ao('a',$cat);
	//generamos el objeto que se insertara en option atributes
	$objeto = new stdClass();
	$objeto->aid = $aid; 
	$objeto->oid = null;
	$objeto->name = $nombre;
	$objeto->cost = 0;
	$objeto->price = 0;
	$objeto->weight = 0;
	if(uc_attribute_option_save($objeto))
	{
		
	//obtenesmos el indice de la opcion que acabamos de crear para almacenarlo en nuestra tabla
		$res = db_query('SELECT MAX(oid) as "oid" FROM {uc_attribute_options}');
		while($row = db_fetch_array($res))
			$oid = $row['oid'];

	
		if($file = file_save_upload('imagen', $validators, file_directory_path().'/muestrario',FILE_EXISTS_RENAME))
		{
			$img = $file->fid;
			file_set_status($file, FILE_STATUS_PERMANENT);
			if(db_query("INSERT INTO {muestitem} (bid,url,img,nombre,aid,oid) values(%d,'%s',%d,'%s',%d,%d)",$cat,$url,$img,$nombre,$aid,$oid) == FALSE)
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
	}
	else
	{
		$salida = 'se genero un problema al agregar la opcion a la tabla uc_attributes_option';
	}
	drupal_set_message($salida);
}

//genera el formulario de catalogo
function muestrario_fcatalogo()
{
	$form['nombre']= array(
		'#type' => 'textfield',
		'#title' => 'nombre',
		'#description' => 'escribe el nombre del catalogo',
		'#required' => TRUE,
	);
	$form['descripcion']= array(
		'#type' => 'textfield',
		'#title' => 'Descripcion',
		'#description' => 'escribe una descripcion del catalogo, sera la misma que aparesca como ayuda en el atributo del mismo nombre',
		'#required' => TRUE,
	);
	$form['submit'] = array(
	'#type' => 'submit',
	'#value' => t('agregar')
	);
	return $form;
}
// procesa el formulario de catalogo
function muestrario_fcatalogo_submit($form, $form_state)
{
	$nombre = $form_state['values']['nombre'];
	$descripcion = $form_state['values']['descripcion'];
	if($nombre != '')
	{
	//generamos un objeto para poder indexar un atributo nuevo en el modulo attributes de ubercart
	$objeto =  new stdClass();
	$objeto->aid = null;
	$objeto->name = $nombre;
	$objeto->label = 'label_'.$nombre;
	$objeto->ordering = 0;
	$objeto->required = 0;
	$objeto->display = 1;
	$objeto->description = $descripcion;
	if(uc_attribute_save($objeto))
		{	
		//obtenesmos el indice del atributo que acabamos de crear para almacenarlo en nuestra tabla
		$res = db_query('SELECT MAX(aid) as "aid" FROM {uc_attributes}');
		while($row = db_fetch_array($res))
			$ind = $row['aid'];
			
			
			if(db_query("INSERT INTO {muestrario} (nombre,descripcion,aid) values('%s','%s',%d)",$nombre,$descripcion,$ind) == FALSE)
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
			$salida = 'no fue posible generar el atributo';
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
	
	drupal_json(array('status' => 0, 'data' => $output));
}
//obtiene el aid (id de atributo) o oid (id de la opcion)
function _get_id_ao($sel,$id)
{
	$aoid = 0;
	switch($sel)
	{
		case 'a':
			$q = 'SELECT aid as "id" FROM {muestrario} WHERE bid = '.$id;
		break;
		case 'o':
			$q = 'SELECT oid as "id" FROM {muestitem} WHERE cid = '.$id;
		break;
	}
	
	$res = db_query($q);
		while($row = db_fetch_array($res))
			$aoid = $row['id'];
			
			
return $aoid;
}
//funcion que elimina elementos de catalog
function _del_muestrario_item_catalogo($id)
{
	$ind = _get_id_ao('a',$id);
	if(uc_attribute_delete($ind))
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
	else
	{
		return FALSE;
	}
}

function _del_muestrario_item_items($id)
{
	//obtenemos el oid para eliminarlo de las opciones de atributo
	$ind = _get_id_ao('o',$id);
	if(uc_attribute_option_delete($ind))
	{
		if(file_delete(_get_muestrario_filepath($id)))
		{
			if(db_query('delete from {files} where fid = (SELECT img FROM {muestitem} WHERE cid = '.$id.')') == FALSE)
			{
				return FALSE;
			}
			else
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
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
}
//funciones auxiliares
//obtener path de una imagen
function _get_muestrario_filepath($id)
{
	$path = '';
	$res = db_query('SELECT filepath FROM {files} WHERE fid = (SELECT img FROM {muestitem} WHERE cid = '.$id.')');
		while($row = db_fetch_array($res))
			$path = $row['filepath'];
			
	return $path;
}

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