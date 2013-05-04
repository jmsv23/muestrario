// JavaScript Document
var div,divR;


function clickDel(){
		var val = this.value;
		var nombre = this.querySelector('span').innerHTML;
		div = document.querySelector('.muest-float');
		$('.muest-float').slideDown(200);
		$('.muest-float').children('p').html('estas seguro que quieres eliminar el elemento "'+nombre+'", se eliminaran sus elementos relacionados en los atributos de producto');
		$('.muest-float').children('#btn-si').attr('value',val);
		$('.muest-float').children('#btn-si-item').attr('value',val);
		$('.muest-float').slideDown(500);
}

function limpiarId(id)
{
	id = id.toString().replace('muest-catalog muest-catalog-','s');
	return id;
}

$(document).ready(function(e) {
		control_listener();
		
		
		//esconder todos los bloques de muestrario
		//$('.muest-catalog').css('display','none');
		
		
		$('select').change(function(){
			var cls = new Array();
			$('.attribute').each(function(index, element) {
                cls.push(this.className);
            });
			
			
			var cat = $('.muest-catalog');
				$('.muestrario-all').html('');
				var reo = new Array();
				for(var i = 0,limit = cat.length; i < limit; i++ )
				{
				reo[limpiarId(cat[i].className)] = cat[i];
				}
			
			
			for(var i = 0,limit = cls.length; i < limit; i++ )
			{
				cls[i] = cls[i].toString().replace('attribute attribute-','');
				cls[i] = cls[i].toString().replace(' even','');
				cls[i] = cls[i].toString().replace(' odd','');
				cls[i] = parseInt(cls[i]);
			}
			console.log(cls);
			cls.forEach(function(value){
				if($('.attribute-'+value).css('display') == 'block')
				{
					$('.muestrario-all').append(reo['s'+value]);
				}
				});
				
			cls.forEach(function(value){
				if($('.attribute-'+value).css('display') != 'block')
				{
					$('.muestrario-all').append(reo['s'+value]);
				}
				});
			});
		
});






function control_listener()
{
	var ctrl = document.querySelectorAll('.cat-erase');
	for(var i = 0, n = ctrl.length; i < n; i++)
	{
		ctrl[i].addEventListener ("click", clickDel, false);
	}
}

function noEliminar()
{
	$('.muest-float').slideUp(500);
}



Drupal.behaviors.muestrario = function(context) {
  $('#btn-si').click(function(){
    $.get('/portal/muestrario/ajax/delete/catalogo/'+this.value , null, tablaResponse);
    return false;
  });
  $('#btn-si-item').click(function(){
	  console.log('se esta haciendo el click')
    $.get('/portal/muestrario/ajax/delete/item/'+this.value , null, tablaResponse);
    return false;
  });
}

var tablaResponse = function(response) {
  var result = Drupal.parseJson(response);  
  $('.muest-float').slideUp(500);
  $('#ajax_catalogo').html(result.data);
  control_listener();
}