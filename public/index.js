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
        alert("Not a real selection!");
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
            $('#blah').html('Could not request file data' + fileName + 'from server');
            console.log(error);
        }
    });
}

function replace_svg_data_in_view(file_data) {
    //Clear the current table
    $("#svg_image_component_table tr").remove();
    $("#component_table_div").attr("style", 'height:300px; overflow:auto;')


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

function replace_svg_image_in_view(file_data) {
    console.log('replacing image...');
    $('#svg_view_image').attr("src", 'uploads/' + file_data.fileName);
}

function replace_rectList_in_view(file_data) {
    if (file_data.rectList.length > 0) {
        console.log('there are rectangles!');
        for (let x in file_data.rectList) {
            console.log(file_data.rectList[x]);

            let row = '<tr>';

            let index = parseInt(x) + 1;
            row += '<td class="nr">Rectangle ' + index + '</td>';

            row += '<td style="text-align:center">';
            row += 'Upper left corner: x = ' + file_data.rectList[x].x + file_data.rectList[x].units + ', y = ' + file_data.rectList[x].y + file_data.rectList[x].units + ', ';
            row += 'Width: ' + file_data.rectList[x].w + file_data.rectList[x].units + ', Height: ' + file_data.rectList[x].h + file_data.rectList[x].units;
            row += '</td>';

            row += '<td>';
            row += file_data.rectList[x].numAttr;
            if (file_data.rectList[x].numAttr > 0) {
                row += '<button type="button" style="margin:5px;"class="btn btn-secondary" onclick="show_attributes(this);">Show</button>';
            }
            row += '</td>';

            row += '</tr>';

            console.log(row);
            $('#svg_image_component_table').append(row);
        }
    } else {
        console.log('there are no rectangles.');
    }
}

function replace_circList_in_view(file_data) {
    if (file_data.circList.length > 0) {
        console.log('there are circles!');
        console.log(file_data.circList);
        for (let x in file_data.circList) {
            let row = '<tr>';

            let index = parseInt(x) + 1;
            row += '<td>Circle ' + index + '</td>';

            row += '<td>';
            row += 'Centre: x = ' + file_data.circList[x].cx + file_data.circList[x].units + ', y = ' + file_data.circList[x].cy + file_data.circList[x].units + ', radius = ' + file_data.circList[x].r + file_data.circList[x].units
            row += '</td>';

            row += '<td>';
            row += file_data.circList[x].numAttr;
            if (file_data.circList[x].numAttr > 0) {
                row += '<button type="button" style="margin:5px;"class="btn btn-secondary" onclick="show_attributes(this);">Show</button>';
            }
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
        for (let x in file_data.pathList) {
            let row = '<tr>';

            let index = parseInt(x) + 1;
            row += '<td>Path ' + index + '</td>'

            row += '<td>';
            row += 'path data = ' + file_data.pathList[x].d;
            row += '</td>';

            row += '<td>';
            row += file_data.pathList[x].numAttr;
            if (file_data.pathList[x].numAttr > 0) {
                row += '<button type="button" style="margin:5px;;"class="btn btn-secondary" onclick="show_attributes(this);">Show</button>';
            }
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
        for (let x in file_data.groupList) {
            let row = '<tr>';

            let index = parseInt(x) + 1;
            row += '<td>Group ' + index + '</td>';

            row += '<td>';
            row += file_data.groupList[x].children + ' child elements';
            row += '</td>';

            row += '<td>';
            row += file_data.groupList[x].numAttr;
            if (file_data.groupList[x].numAttr > 0) {
                row += '<button type="button" style="margin:5px;"class="btn btn-secondary" onclick="show_attributes(this);">Show</button>';
            }
            row += '</td>';

            row += '</tr>';

            console.log(row);
            $('#svg_image_component_table').append(row);
        }
    } else {
        console.log('there are no groups.')
    }
}

function show_attributes(element) {
    var value = $('#image_dropdown').val();
    if (value.localeCompare("") == 0) {
        console.log('bad input how did this happen!?')
    }

    var component = $(element).closest('tr').find('td:first').text();
    
    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/get_component_data',
        data: {
            fileName: value,
            componentType: component.split(" ")[0],
            index: component.match(/\d+/)[0],    
        },
        success: function (component_data) {
            ret_str = JSON.stringify(component_data.data, null, 2).replace(/\"/g, '')
                                                                  .replace(/{/g, '')
                                                                  .replace(/}/g, '')
                                                                  .replace(/[\[\]']+/g, '')
                                                                  .replace(/,/g, '');
            console.log("received " + ret_str);
            alert(ret_str);
        },
        fail: function (error) {
            $('#blah').html('Could not request file data' + fileName + 'from server');
            console.log(error);
            alert(error);
        }
    });
}

function append_html_to_image_dropdown(files) {
    if (files.length == 0) {
        console.log("no files for dropdown!");
        return;
    }

    for (let i = 0; i < files.length; i += 1) {
        let option = '<option>' + files[i].fileName + '</option>'
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

function edit_svg() {
    //get edit dropdown value
    let edit_dropdown_value = $("#edit_dropdown :selected").text();
    console.log("editing dropdown value: " + edit_dropdown_value);
    if (edit_dropdown_value.localeCompare("") == 0) {
        console.log("not valid edit value");
        alert("not valid edit value");
        return;
    }

    //get svg view drowpdown value
    let svg_view_dropdown_value = $("#image_dropdown").val();
    console.log(svg_view_dropdown_value);
    if (svg_view_dropdown_value.localeCompare("") == 0) {
        console.log("no svg image selected!");
        alert("no svg image selected!");
        return;
    }

    //get text
    let edit_text = $("#entry_box").val();
    if (edit_text.localeCompare("") == 0) {
        console.log("no text inputed!");
        alert("no text inputed!");
        return;
    }

    console.log("ready to send edit request!");
    //send request to server to edit specified svg image
    $.ajax({
        async: false,
        type: 'get',
        dataType: 'json',
        url: '/send_edit',
        data: {
            fileName: svg_view_dropdown_value,
            editValue: edit_dropdown_value,
            editText: edit_text,
        },
        success: function (status) {
            console.log(status);
            location.reload();
        },
        fail: function (error) {
            $('#blah').html('Could not request edit to server');
            console.log(error);
        }
    });
}

function create_svg_image() {
    console.log("attempting to make new image");
    file_name_from_form = $("#file_name_entry_box").val();
    console.log(file_name_from_form);
    if (file_name_from_form.localeCompare("") == 0) {
        console.log("bad file name input");
        alert("bad file name input");
        return;
    }

    $.ajax({
        async: false,
        type: 'get',
        dataType: 'json',
        url: '/create_image',
        data: {
            fileName: file_name_from_form
        },
        success: function (status) {
            console.log(status);
            location.reload();
        },
        fail: function (error) {
            console.log('Could not request making new image on server');
            console.log(error);
        }
    });

    // location.reload();
}

function add_shape_to_svg() {
    //get svg view drowpdown value
    let selected_image = $("#image_dropdown").val();
    console.log(selected_image);
    if (selected_image.localeCompare("") == 0) {
        console.log("no svg image selected!");
        alert("no svg image selected!");
        return;
    }

    let colour_selection = $('#add_shape_text_box').val();
    console.log(colour_selection);
    if (colour_selection.localeCompare("") == 0) {
        console.log('no Colour!');
        alert("no colour");
        return;
    }

    let shape_selection = $('#add_shapes_dropdown').val();
    console.log(shape_selection);

    if (shape_selection.localeCompare("Rectangle") == 0) {
        let x = $('#add_shape_x_val_input').val();
        let y = $('#add_shape_y_val_input').val();
        let w = $('#add_shape_width_input').val();
        let h = $('#add_shape_height_input').val();
        let u = $('#units').val();

        if (!x || !y || !w || !h || !u) {
            console.log('bad rectangle input!');
            alert("bad rectangle input");
            return;
        }

        let rect_str = '{"x":' + x + ',"y":' + y + ',"w":' + w + ',"h":' + h + ',"units":"' + u + '"}';
        console.log(rect_str);

        console.log('ready to send shape!')
        $.ajax({
            async: false,
            type: 'get',
            dataType: 'json',
            url: '/add_shape',
            data: {
                fileName: selected_image,
                shape: shape_selection,
                data: rect_str,
                colour: colour_selection,
            },
            success: function (status) {
                console.log(status);
            },
            fail: function (error) {
                console.log('Could not request adding rectangle on server');
                console.log(error);
            }
        });
    } else if (shape_selection.localeCompare("Circle") == 0) {
        let cx = $('#add_shape_cx_val_input').val();
        let cy = $('#add_shape_cy_val_input').val();
        let r = $('#add_shape_r_val_input').val();
        let u = $('#units').val();

        if (!cx || !cy || !r || !u) {
            console.log('bad circle input values');
            alert('bad circle input values');
            return;
        }

        let circ_str = '{"cx":' + cx + ',"cy":' + cy + ',"r":' + r + ',"units":"' + u + '"}';
        console.log(circ_str);

        $.ajax({
            async: false,
            type: 'get',
            dataType: 'json',
            url: '/add_shape',
            data: {
                fileName: selected_image,
                shape: shape_selection,
                data: circ_str,
                colour: colour_selection,
            },
            success: function (status) {
                console.log(status);
            },
            fail: function (error) {
                console.log('Could not request adding a circle on server');
                console.log(error);
            }
        });
    } else {
        console.log('invalid selection of shape');
        alert('invalid selection of shape');
        return;
    }

    location.reload();
}

function update_add_shape_input(value) {
    if (value == '') {
        console.log('Not a real selection!');
        alert('Not a real selection!');
        return;
    }
    console.log("new dropdown to add image selected: " + value);

    $('#add_shape_div').empty();

    if (value.localeCompare("Rectangle") == 0) {
        let r_input = '<form ref="add_svg_image_form_data" id="add_svg_image_form_data">';
        r_input += '<div class="form-group"><input type="text" class="form-control" id="add_shape_text_box" placeholder="Enter Shape Colour"></div>';
        r_input += '</form>';

        r_input += '<label for="x_val">X-Val:</label><input type="number" id="add_shape_x_val_input" name="x_val" min="-10000" max="10000" style="margin:5px;">';

        r_input += '<label for="y_val">Y-Val:</label><input type="number" id="add_shape_y_val_input" name="y_val" min="-10000" max="10000" style="margin:5px;">';

        r_input += '<label for="width_val">Width:</label><input type="number" id="add_shape_width_input" name="width_val" min="0" max="10000" style="margin:5px;">';

        r_input += '<label for="height_val">Height:</label><input type="number" id="add_shape_height_input" name="height_val" min="0" max="10000" style="margin:5px;">';

        r_input += '<label for="units">Units:</label><input type="text" id="units" name="units" maxlength="6" size="6" style="margin:5px;">';

        r_input += '<div><button onclick="add_shape_to_svg()" class="btn btn-secondary">Submit</button></div>';

        $('#add_shape_div').append(r_input);
    } else if (value.localeCompare("Circle") == 0) {
        let c_input = '<form ref="add_svg_image_form_data" id="add_svg_image_form_data">';
        c_input += '<div class="form-group"><input type="text" class="form-control" id="add_shape_text_box" placeholder="Enter Shape Colour"></div>';
        c_input += '</form>';

        c_input += '<label for="cx_val">cx:</label><input type="number" id="add_shape_cx_val_input" name="cx_val" min="-10000" max="10000" style="margin:5px;">';

        c_input += '<label for="cy_val">cy:</label><input type="number" id="add_shape_cy_val_input" name="cy_val" min="-10000" max="10000" style="margin:5px;">';

        c_input += '<label for="r_val">r:</label><input type="number" id="add_shape_r_val_input" name="r_val" min="0" max="10000" style="margin:5px;">';

        c_input += '<label for="units">Units:</label><input type="text" id="units" name="units" maxlength="6" size="6" style="margin:5px;">';

        c_input += '<div><button onclick="add_shape_to_svg()" class="btn btn-secondary">Submit</button></div>';

        $('#add_shape_div').append(c_input);
    } else {
        console.log("Bad dropdown input");
    }
}

function edit_attribute() {
    console.log("request for editing attribtue");

    let selected_image = $("#image_dropdown").val();
    console.log(selected_image);
    if (selected_image.localeCompare("") == 0) {
        console.log("no svg image selected!");
        alert("no svg image selected!");
        return;
    }
    
    //get shape selection
    let component_selection = $('#attribute_edit_dropdown').val();
    if (component_selection.localeCompare("") == 0) {
        console.log("not a valid component selection");
        alert("not a valid component selection");
    }

    let selected_index = $('#attribute_edit_index').val();
    
    let attribute_name = $('#attribute_name_edit_entry_box').val();
    
    let attribute_value = $('#attribute_value_edit_entry_box').val();

    $.ajax({
        async: false,
        type: 'get',
        dataType: 'json',
        url: '/edit_attribute',
        data: {
            fileName: selected_image,
            componentType: component_selection,
            index: selected_index,
            attributeName: attribute_name,
            attributeValue: attribute_value,
        },
        success: function (status) {
            console.log(status);
            location.reload();
        },
        fail: function (error) {
            console.log('Could not request adding a circle on server');
            console.log(error);
        }
    });
}

function scale_shapes() {
    //get svg image selection
    let selected_image = $("#image_dropdown").val();
    if (selected_image.localeCompare("") == 0) {
        console.log("no svg image selected!");
        alert("no svg image selected!");
        return;
    }
    
    //get shape selection
    let component_selection = $('#scale_shapes_dropdown').val();
    if (component_selection.localeCompare("") == 0) {
        console.log("not a valid component selection");
        alert("not a valid component selection");
    }

    //get factor 
    let scale_factor_selection = $('#factor_shapes_value').val();

    console.log("selected_image: " + selected_image + " shape selection: " + component_selection + "factor: " + scale_factor_selection);

    $.ajax({
        async: false,
        type: 'get',
        dataType: 'json',
        url: '/scale_components',
        data: {
            fileName: selected_image,
            componentType: component_selection,
            factor: scale_factor_selection,
        },
        success: function (status) {
            console.log(status);
            location.reload();
        },
        fail: function (error) {
            console.log('Could not request adding a circle on server');
            console.log(error);
        }
    });
}