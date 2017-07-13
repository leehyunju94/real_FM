var express = require('express')

var app = express()
var router = require('./routes/main')(app)

app.set('views', __dirname + '/views');
app.set('view engine', 'ejs');
app.engine('html', require('ejs').renderFile);
app.use(express.static('public'))
app.use('/files', express.static('uploads'))

var server = app.listen( 3000, () => {
  console.log('Server is Running on port 3000');
})
