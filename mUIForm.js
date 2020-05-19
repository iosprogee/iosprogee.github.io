//Firebase configuration
var firebaseConfig = {
  apiKey: "AIzaSyDMSdTQr8Y-FZWMgFMvb3tDyRf_y44KYis",
  authDomain: "fireupmyapp.firebaseapp.com",
  databaseURL: "https://fireupmyapp.firebaseio.com",
  projectId: "fireupmyapp",
  storageBucket: "fireupmyapp.appspot.com",
  messagingSenderId: "823746792317",
  appId: "1:823746792317:web:efbf29d7dc522a5a329a14",
  measurementId: "G-M08RXEKFFQ"
};
let fb=firebase;
fb.initializeApp(firebaseConfig);
fb.analytics();

let d=document;
d.title = "CEF"

var l = d.querySelector("link[rel*='icon']") || d.createElement('link');
l.type = 'image/x-icon'; l.rel = 'shortcut icon'; 
l.href = 'https://iosprogee.github.io/favicon.ico';
d.getElementsByTagName('head')[0].appendChild(l);

d.querySelector('meta[name="apple-mobile-web-app-title"]').setAttribute("content",  d.title);


const aFields=['id','name','email','phone','remarks'];
function clearInputs(aF=aFields,focal){
  focal=focal || "name";
  aFields.forEach((el) => d.getElementById(el).value = '');
  d.getElementById(focal).focus();
}

const allLabels=d.getElementsByTagName("label");
function removeLabels(){
  //remove all labels
  Object.keys(allLabels).forEach((item)=>{
    let firstGen=allLabels[item].childNodes;  //console.log(firstGen);
    for (var i = 0; i < firstGen.length; i++) {
      if(firstGen[i].nodeName='#text') firstGen[i].nodeValue=''; 
      //if(firstGen[i].nodeName='I') //nodeName for icon is I
    }
  });
}

window.addEventListener('load',()=>{   
  const allInputs=d.getElementsByTagName("input");
  /*
  Object.keys(allInputs).forEach((item)=>{
    console.log(allInputs[item].id);
  });    
  const allTA=d.getElementsByTagName("textarea");
  Object.keys(allTA).forEach((item)=>{
    console.log(allTA[item].id);
  }); 
  */

  removeLabels();
  //d.getElementById("saved").classList.remove("hide")
  //d.getElementById("saved").classList.add("hide")
  d.getElementById("name").focus()

  d.getElementById('clear').addEventListener('click',(e)=>{
    clearInputs();
  });

  d.getElementById('save').addEventListener('click',(e)=>{
    writeData2FB();
  });    
/*
  var idHide=true;
  d.getElementById('check').addEventListener('click',(e)=>{
    if(idHide) d.getElementById("id-row").classList.remove('hide')
    else d.getElementById("id-row").classList.add('hide');
    idHide=!idHide;    
  });
*/
  d.getElementById('search').addEventListener('click',(e)=>{
    if(d.getElementById('name').checkValidity()){
      fb.database().ref('customer').orderByChild("name")
      .equalTo(d.getElementById('name').value).once("value",snap => {
        clearInputs();          
        if (snap.exists()){
          let record = Object.entries(snap.val())[0]
          console.log(`key: ${record[0]}, name: ${record[1].name}`)
          //for(var propertyName in record) console.log(propertyName);
          aFields.forEach((el) => {
            if(el=='id') d.getElementById(el).value = record[0]
            else  d.getElementById(el).value = record[1][el];
          });

        }  
      });

    }
    e.preventDefault(); 
  })

  d.getElementById('submit').addEventListener('click',(e)=>{
    //if(d.getElementById('name').checkValidity() && d.getElementById('phone').checkValidity() && d.getElementById('email').checkValidity())  
    writeData2FB();
    e.preventDefault(); 
  })

}, false);  //eo window.addEventListener('load')

function writeData2FB(refDB = 'customer', arrFields = aFields) {
  var id = d.getElementById("id").value || fb.database().ref(refDB).push().key;
  d.getElementById("id").value = id;

  fb.database().ref(refDB).orderByKey().equalTo(id).once("value",snapshot => {
    if (snapshot.exists()) console.log(`${refDB} existing.`, snapshot.val())
    else console.log(`${refDB} new.`);

    const objRec = arrFields.reduce((acc,fName)=>{
      return Object.assign( {}, acc, 
        fName !== "id" ? { [fName]:d.getElementById(fName).value } : {});
    }, {}
    );

    fb.database().ref(refDB + '/' + id).update(objRec);
/*
    fb.database().ref(refDB + '/' + id).update({
      "name": d.getElementById("name").value,
      "email": d.getElementById("email").value,
      "phone": d.getElementById("phone").value,
      "remarks": d.getElementById("remarks").value
    }).then((snap)=>{
      console.log('record saved.');      
    });
*/
  });

}

/*

  await fb.database().ref('customer').once("value").then(snap => {
    snap.forEach( (item)=> {
      var i = item; var c = item.val();
      console.log(i.key, c.name, c.email, c.phone)
    })  
  })

  d.getElementById('id').value = record[0];
  d.getElementById('name').value = record[1]["name"];
  d.getElementById('email').value = record[1]["email"];
  d.getElementById('phone').value = record[1]["phone"];
  d.getElementById('remarks').value = record[1]["remarks"];

*/    

