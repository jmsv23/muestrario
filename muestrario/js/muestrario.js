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



$(document).ready(function(e) {
		control_listener();
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