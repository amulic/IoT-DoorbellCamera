
    
    const firebaseConfig = {

    apiKey: "",
  
    authDomain: "",
  
    databaseURL: "",
  
    projectId: "",
  
    storageBucket: "",
  
    messagingSenderId: "",
  
    appId: "",
  
    measurementId: ""
  
    };

    //telegramBot
    const botToken = '';
    const chatId = '';
    const message = 'YOUR_MESSAGE';

    const apiUrl = `YOUR_API_URL`;

  firebase.initializeApp(firebaseConfig);
  //buttons
  const startButton=document.getElementById("startButton");
  const captureButton=document.getElementById("captureButton");
  //login
  const email=document.getElementById("email");
  const password=document.getElementById("password");
  const toggleButton = document.getElementById('toggleButton');
  //Camera    
  const image=document.getElementById("image");
  const container=document.getElementById("container");
  
  //firebase
  const db = firebase.database();
  const auth = firebase.auth();
  

function display() {
  container.style.display = 'inline';
}
 
  
  
    
    function peek() {
        db.ref('images').on('value', snap=>{
        image.src='data:image/jpeg;'+snap.val().substring(5);  
        });

        //slanje poruke na telegram
        fetch(apiUrl, {
            method: 'POST',
            headers: {
              'Content-Type': 'application/json',
            },
            body: JSON.stringify({
              chat_id: chatId,
              text: message,
            }),
          })
            .then(response => {
              if (!response.ok) {
                throw new Error('Error sending message: ' + response.statusText);
              }
              return response.json();
            })
            .then(data => {
              console.log('Message sent successfully:', data);
            })
            .catch(error => {
              console.error(error.message);
            });
    }

    auth.onAuthStateChanged((user)=>{
      if(user){
          display();
          startButton.onclick=function(){
              db.ref('started').set(true);
          }
          stopButton.onclick=function(){
              db.ref('started').set(false);
          }
      }
      else{
           display();
        }
    })

    //skidanje slike
  
    function downloadImage() {
        //dobivanje sourca
        var imageUrl = document.getElementById("image").src;

        var downloadLink = document.createElement("a");
        downloadLink.href = imageUrl;
        downloadLink.download = "downloaded_image.jpg";
        document.body.appendChild(downloadLink);
        downloadLink.click();
        document.body.removeChild(downloadLink);
    }
/*
    toggleButton.addEventListener('click', () => {
        //Read the 'ledControl' value from Firebase
        firebase.database().ref('/ledControl').once('value').then((snapshot) => {
          const ledControl = snapshot.val();
  
          //Update the LED status in Firebase
          firebase.database().ref().update({
            ledControl: !ledControl
          });
        });
    });
*/
