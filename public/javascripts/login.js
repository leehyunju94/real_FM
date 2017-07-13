Kakao.init('5c2b7a24b9a3afc18dde8e77c51791b6')

var data

function loginWithKakao() {
 Kakao.Auth.login({
   success: function(authObj) {
     Kakao.API.request({
       url: '/v1/user/me',
       success: (res) => {
         console.log(res)
         alert(res.properties.nickname+'님 환영합니다. ')
         fetch(`/user/${res.properties.nickname}`, {
           method: 'POST',
           body: res.properties,
         })
         fetch('/')

       },
       fail: (err) => {
         alert(JSON.stringify(err));
       }
     })
    // alert(JSON.stringify(authObj));
   },
   fail: function(err) {
     alert(JSON.stringify(err));
   }
 });
}
