module.exports = ( app, fs ) => {

  var multer = require('multer')
  var express = require('express')
  var fs = require('fs')
  var util = require('util')
  var path = require('path')
  var mime = require('mime')
  var mysql = require('mysql')
  var bodyParser = require('body-parser')



  var connection = mysql.createConnection({
    host: 'localhost',
    user: 'root',
    password: 'fontmaker',
    database: 'fontmaker',
  })

  connection.connect( (err) => {
    if( err ){
      console.error('mysql connection error')
      console.error(err)
      throw err
    }
    else {
      console.log('연결에 성공하였습니다.')
    }
  })

  //파일 업로드 코드
  var storage = multer.diskStorage({
    destination: ( req, file, cb ) => {
      cb( null, 'uploads/')
    },
    filename: ( req, file, cb ) => {
      cb( null, file.originalname )
    },
  })
  var upload = multer({
    storage: storage
  })

  app.get( '/', ( req, res ) => {
    res.render( 'index' )
  })

  app.post('/user/:nickname', ( req, res ) => {
    console.log(req.params.nickname)
    console.log(req.body)
    var sql = `INSERT INTO user ()`
    res.send()
  })

  app.get( '/upload', ( req, res ) => {
    res.render( 'upload' )
  })

  app.get( '/download/:field', ( req, res ) => {
    var field = req.params.field
    var origFileName, savedFileName, savedPath, fileSize
    if( field == '1' ){
      origFileName = '1111.jpg'
      savedFileName = '1.jpg'
      savedPath = 'C:/Users/merii/Documents/GitHub/fontmaker/server/uploads'
      fileSize = ''
    }
    else if( field == '2' ){
      origFileName = '2222.jpg'
      savedFileName = '2.jpg'
      savedPath = 'C:/Users/merii/Documents/GitHub/fontmaker/server/uploads'
      fileSize = ''
    }
    else if( field == '3' ){
      origFileName = 'report.docx'
      savedFileName = '1report.docx'
      savedPath = 'C:/Users/merii/Documents/GitHub/fontmaker/server/uploads'
      fileSize = ''
    }
    else if( field == '4' ){
      origFileName = 'kimering.otf'
      savedFileName = 'own.otf'
      savedPath = 'C:/Users/merii/Documents/GitHub/fontmaker/server/uploads'
      fileSize = ''
    }

    var file = savedPath + '/' + savedFileName
    mimetype = mime.lookup(origFileName)

    res.setHeader( 'Content-disposition', 'attachment; filename=' + origFileName )
    res.setHeader( 'Content-type', mimetype )

    var filestream = fs.createReadStream( file )
    filestream.pipe(res)

  })

  app.post( '/upup', upload.single('userfile'), ( req, res ) => {
    res.send( req.file )
    console.log(req.file)
  })

  app.get( '/font', ( req, res ) => {
    res.render( 'font.html')
  })

  app.get('/login', ( req, res ) => {
    res.render('login.html')
  })



}
