
var Module = {
    onRuntimeInitialized: function() {
        document.getElementById("button-format").removeAttribute("disabled");
        console.log("Formatter is ready!");
    }
};

function formatList() {
    inputText = document.getElementById("input").value;
    var result = Module.MakeOptionsAndReformat(
            inputText,
            // Input options
            document.getElementById("has-double-quotes").checked,
            document.getElementById("has-single-quotes").checked,
            document.getElementById("has-single-spaces").checked,
            document.getElementById("has-commas").checked,
            document.getElementById("has-newlines").checked,
            document.getElementById("has-trailing-delimiter").checked,
            // Output options
            document.getElementById("add-double-quotes").checked,
            document.getElementById("add-single-quotes").checked,
            document.getElementById("add-commas").checked,
            document.getElementById("add-spaces").checked,
            document.getElementById("add-newlines").checked,
            document.getElementById("add-trailing-delimiter").checked,
            document.getElementById("sort-output").checked);
    document.getElementById("output").value = result;
}

// TODO: Implement support for variable-number spaces.

// TODO: Add "copy" button to index.html when HTTPS is supported. The Clipboard
//       API is only available over HTTPS contexts.
//       (https://developer.mozilla.org/en-US/docs/Web/API/Clipboard)
function writeOutputToClipboard() {
    navigator.clipboard.writeText(document.getElementById("output").value);
}

