// Put all onload AJAX calls here, and event listeners
$(document).ready(function () {
    // On page-load AJAX Example
    add_elements_to_file_log_table();

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
            $('#blah').html("On page load, received string '" + data.foo + "' from server");
            //We write the object to the console to show that the request was successful
            console.log(data);

        },
        fail: function (error) {
            // Non-200 return, do something with error
            $('#blah').html("On page load, received error from server");
            console.log(error);
        }
    });

    // Event listener form example , we can use this instead explicitly listening for events
    // No redirects if possible
    $('#someform').submit(function (e) {
        $('#blah').html("Form has data: " + $('#entryBox').val());
        e.preventDefault();
        //Pass data to the Ajax call, so it gets passed to the server
        $.ajax({
            //Create an object for connecting to another waypoint
        });
    });
});

function appendLogToHtml(files) {
    // console.log(files);
    // console.log(files.length);
    if (files.length == 0) {
        console.log('No files!')
        let row = '<tr> <td style="text-align:center">No Files O_o </td></tr>';
        $('#file_log_table').append(row);
        return;
    }

    for (let i = 0; i < files.length; i += 1) {
        let row = "<tr>";

        row += '<td><a download="' + files[i].fileName + '" href="uploads/' + files[i].fileName + '"> <img src=uploads/' + files[i].fileName + ' border="3" height="150" width="150"> </a></td>';

        // console.log(files[i]);
        // console.log(row); 

        row += '<td style="text-align:center"><a download="' + files[i].fileName + '" href="uploads/' + files[i].fileName + '">' + files[i].fileName + '</a></td>';

        // console.log(files[i].size);

        row += '<td style="text-align:center">' + files[i].size + 'KB</td>';

        row += '<td style="text-align:center">' + files[i].SVGdata.numRect + '</td>';

        row += '<td style="text-align:center">' + files[i].SVGdata.numCirc + '</td>';

        row += '<td style="text-align:center">' + files[i].SVGdata.numPaths + '</td>';

        row += '<td style="text-align:center">' + files[i].SVGdata.numGroups + '</td>';

        row += '</tr>';

        $('#file_log_table').append(row);
    }
}

function add_elements_to_file_log_table() {
    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/getFiles',
        success: function (directory) {
            appendLogToHtml(directory);
        },
        fail: function (error) {
            $('#blah').html("On page load, received error from server");
            console.log(error);
        }
    });
}