// Put all onload AJAX calls here, and event listeners
$(document).ready(function() {
    // On page-load AJAX Example
    
    //!how to do this dynamically, ie making row elements
    var sTempTableRow = '<tr><td>cell-1 </td><td>cell-2 </td><td>cell-3 </td><td>cell-4 </td></tr>'
    $('#file_log_table').append(sTempTableRow);



    $.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything 
        url: '/someendpoint',   //The server endpoint we are connecting to
        data: {
            name1: "Value 1",
            name2: "Value 2"
        },
        success: function (data) {
            /*  Do something with returned object
                Note that what we get is an object, not a string, 
                so we do not need to parse it on the server.
                JavaScript really does handle JSONs seamlessly
            */
            $('#blah').html("On page load, received string '"+data.foo+"' from server");
            //We write the object to the console to show that the request was successful
            console.log(data); 

        },
        fail: function(error) {
            // Non-200 return, do something with error
            $('#blah').html("On page load, received error from server");
            console.log(error); 
        }
    });

function add_elements_to_file_log_table() {

    var newRow = tableRef.newRow();
    
    var newCell = newRow.insertCell(0);

    var newText  = document.createTextNode('New row');

    newCell.appendChild(newText);
    
    /* var body = document.body,
        tbl  = document.createElement('table');
    tbl.style.width  = '100%';
    tbl.style.border = '1px solid black'; */

    /* for(var i = 0; i < 3; i++) {
        var tr = tbl.insertRow();
        for(var j = 0; j < 2; j++) {
            var td = tr.insertCell();
            td.appendChild(document.createTextNode('Cell'));
            td.style.border = '1px solid black';
        }
    } */
}

    // Event listener form example , we can use this instead explicitly listening for events
    // No redirects if possible
    $('#someform').submit(function(e){
        $('#blah').html("Form has data: "+$('#entryBox').val());
        e.preventDefault();
        //Pass data to the Ajax call, so it gets passed to the server
        $.ajax({
            //Create an object for connecting to another waypoint
        });
    });
});