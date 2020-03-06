// Put all onload AJAX calls here, and event listeners
$(document).ready(function () {
    // On page-load AJAX Example
    add_files_to_html();

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

function append_html_to_file_log(files) {
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

            row += '<td><a download="' + files[i].fileName + '" href="uploads/' + files[i].fileName + '"> <img src=uploads/' + files[i].fileName + ' border="3" height="100" width="100"> </a></td>';

            row += '<td style="text-align:center"><a download="' + files[i].fileName + '" href="uploads/' + files[i].fileName + '">' + files[i].fileName + '</a></td>';

            row += '<td style="text-align:center">' + files[i].size + ' KB</td>';

            row += '<td style="text-align:center">' + files[i].SVGdata.numRect + '</td>';

            row += '<td style="text-align:center">' + files[i].SVGdata.numCirc + '</td>';

            row += '<td style="text-align:center">' + files[i].SVGdata.numPaths + '</td>';

            row += '<td style="text-align:center">' + files[i].SVGdata.numGroups + '</td>';

        row += '</tr>';

        $('#file_log_table').append(row);
    }
}

function fill_svg_image_data(value) {
    if (value == '') {
        console.log('Not a real selection!');
        return;
    }
    console.log("new dropdown image selected: " + value);

    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/getFileData',
        data: {
            fileName: value,
        },
        success: function (file_data) {
            console.log("received " + file_data.fileName);
            console.log(file_data.fileName);
            console.log(file_data.title);
            console.log(file_data.description);
            console.log(file_data.rectList);
            console.log(file_data.circList);
            console.log(file_data.pathList);
            console.log(file_data.groupList);
            console.log(file_data.attrList);

            replace_svg_data_in_view(file_data);
        },
        fail: function (error) {
            $('#blah').html('Could not request file data' + filename + 'from server');
            console.log(error);
        }
    });
}

function replace_svg_data_in_view(file_data) {
    //Clear the current table
    $("#svg_image_component_table tr").remove()

    if (file_data.title.localeCompare("") == 0) {
        $('#title_cell').text("No title");
    } else {
        $('#title_cell').text(file_data.title);
    }

    if (file_data.description.localeCompare("") == 0) {
        $('#desc_cell').text("No Description");
    } else {
        $('#desc_cell').text(file_data.description);
    }

    replace_svg_image_in_view(file_data);

    replace_rectList_in_view(file_data);
    
    replace_circList_in_view(file_data);

    replace_pathList_in_view(file_data);

    replace_groupList_in_view(file_data);
}

function replace_rectList_in_view(file_data) {
    if (file_data.rectList.length > 0) {
        console.log('there are rectangles!');
        for (x in file_data.rectList) {
            console.log(file_data.rectList[x]);
            
            let row = '<tr>';
            
                let index = parseInt(x) + 1;
                row += '<td>Rectangle '+ index +'</td>';
                
                row += '<td style="text-align:center">';
                    row += 'Upper left corner: x = ' + file_data.rectList[x].x + file_data.rectList[x].units + ', y = ' + file_data.rectList[x].y + file_data.rectList[x].units + ', ';
                    row += 'Width: ' + file_data.rectList[x].w + file_data.rectList[x].units + ', Height: ' + file_data.rectList[x].h + file_data.rectList[x].units;
                row += '</td>';
                
                row += '<td>';
                    row += file_data.rectList[x].numAttr;
                row += '</td>';
                
            row += '</tr>';
            
            console.log(row);
            $('#svg_image_component_table').append(row);
        }
    } else {
        console.log('there are no rectangles.');
    }
}

function replace_svg_image_in_view(file_data) {
    console.log('replacing image...');
    $('#svg_view_image').attr("src", 'uploads/' + file_data.fileName);
}

function replace_circList_in_view(file_data) {
    if (file_data.circList.length > 0) {
        console.log('there are circles!');
        console.log(file_data.circList);
        for (x in file_data.circList) {        
            let row = '<tr>';
            
                let index = parseInt(x) + 1;
                row += '<td>Circle '+ index +'</td>';

                row += '<td>';
                    row += 'Centre: x = ' + file_data.circList[x].cx + file_data.circList[x].units + ', y = ' + file_data.circList[x].cy + file_data.circList[x].units + ', radius = ' + file_data.circList[x].r + file_data.circList[x].units
                row += '</td>';

                row += '<td>';
                    row += file_data.circList[x].numAttr;
                row += '</td>';

            row += '</tr>';
            console.log(row);
            $('#svg_image_component_table').append(row);
        }
    } else {
        console.log("there are no circles.");
    }
}

function replace_pathList_in_view(file_data) {
    if (file_data.pathList.length > 0) {
        console.log('there are paths!');
        console.log(file_data.pathList);
        for (x in file_data.pathList) {
            let row = '<tr>';

                let index = parseInt(x) + 1;
                row += '<td>Path ' + index + '</td>'

                row += '<td>';
                    row += 'path data = ' + file_data.pathList[x].d;
                row += '</td>';
                
                row += '<td>';
                    row += file_data.pathList[x].numAttr;
                row += '</td>';

            row += '</tr>'

            console.log(row);
            $('#svg_image_component_table').append(row);
        }
    } else {
        console.log('there are no paths.');
    }
}

function replace_groupList_in_view(file_data) {
    if (file_data.groupList.length > 0) {
        console.log('there are groups!');
        console.log(file_data.groupList);
        for (x in file_data.groupList) {
            let row = '<tr>';

                let index = parseInt(x) + 1;
                row += '<td>Group ' + index + '</td>';

                row += '<td>';
                    row += file_data.groupList[x].children + ' child elements';
                row += '</td>';
                
                row += '<td>';
                    row += file_data.groupList[x].numAttr;
                row += '</td>';

            row += '</tr>';

            console.log(row);
            $('#svg_image_component_table').append(row);
        }
    } else {
        console.log('there are no groups.')
    }
}

function append_html_to_image_dropdown(files) {
    if (files.length == 0) {
        console.log("no files for dropdown!");
        return;
    }

    for (let i = 0; i < files.length; i+=1) {
        let option = '<option>' + files[i].fileName +'</option>'
        $('#image_dropdown').append(option);
    }
}

function add_files_to_html() {
    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/getFiles',
        success: function (directory) {
            append_html_to_file_log(directory);
            append_html_to_image_dropdown(directory);
        },
        fail: function (error) {
            $('#blah').html("On page load, received error from server");
            console.log(error);
        }
    });
}