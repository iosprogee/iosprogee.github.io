var fb=firebase;fb.initializeApp({apiKey:"AIzaSyDMSdTQr8Y-FZWMgFMvb3tDyRf_y44KYis",authDomain:"fireupmyapp.firebaseapp.com",databaseURL:"https://fireupmyapp.firebaseio.com",projectId:"fireupmyapp",storageBucket:"fireupmyapp.appspot.com",messagingSenderId:"823746792317",appId:"1:823746792317:web:efbf29d7dc522a5a329a14",measurementId:"G-M08RXEKFFQ"});var d=document;d.title="CEF";var l=d.querySelector("link[rel*='icon']")||d.createElement("link");l.type="image/x-icon";l.rel="shortcut icon";l.href="https://iosprogee.github.io/favicon.ico";d.getElementsByTagName("head")[0].appendChild(l);d.querySelector('meta[name="apple-mobile-web-app-title"]').setAttribute("content",d.title);d.querySelector('meta[name="application-name"]').setAttribute("content",d.title);d.onkeypress=function(e){if(13==(e=e||window.event).keyCode){e.preventDefault();var t=Array.prototype.slice.call(document.querySelectorAll("input,textarea")),a=(t.indexOf(document.activeElement)+1)%t.length;t[a].focus()}};var aFields="id,name,email,phone,remarks".split(",");function clearInputs(e,t){aFields;t=t||"name";aFields.forEach((function(e){d.getElementById(e).value=""}));d.getElementById(t).focus()}var allLabels=d.getElementsByTagName("label");window.addEventListener("load",(function(){d.getElementsByTagName("input");Object.keys(allLabels).forEach((function(e){for(var t=allLabels[e].childNodes,a=0;a<t.length;a++)(t[a].nodeName="#text")&&(t[a].nodeValue="")}));d.getElementById("name").focus();d.getElementById("clear")&&d.getElementById("clear").addEventListener("click",(function(e){clearInputs()}));divList=d.getElementById("div-list");divForm=d.getElementById("div-form");d.getElementById("list").addEventListener("click",(function(e){if(divList.classList.contains("hide")){divList.classList.remove("hide");divForm.classList.add("hide")}else{divForm.classList.remove("hide");divList.classList.add("hide")}}));d.getElementById("save").addEventListener("click",(function(e){writeData2FB()}));d.getElementById("search").addEventListener("click",(function(e){d.getElementById("name").checkValidity()&&fb.database().ref("customer").orderByChild("name").equalTo(d.getElementById("name").value).once("value",(function(e){clearInputs();if(e.exists()){var t=Object.entries(e.val())[0];console.log(`key: ${t[0]}, name: ${t[1].name}`);aFields.forEach(e=>{d.getElementById(e).value="id"==e?t[0]:t[1][e]})}}));e.preventDefault()}));d.getElementById("submit").addEventListener("click",e=>{writeData2FB();e.preventDefault()});!function(e="customer"){!function(e){e.once("value",(function(e){snapCounter=e.numChildren();console.log("dbSync: "+snapCounter)}));e.on("child_added",(function(e){!function(e,t){var a=document.createElement("div");a.innerHTML=function(e){return'<div class="id-'+e.key+' row list"><span class="cell"><h3 class="name">Robert</h3><span class="material-icons blue" type="x-small">phone</span><span class="phone" class="detail">9999 8888</span><span class="material-icons blue" type="x-small">email</span><span class="email" class="detail">r@robert.io</span></span><span class="cell vertical clicker-'+e.key+'"><span class="material-icons blue medium right">keyboard_arrow_right</span></span></div>'}(e);var n=a.firstChild,i=e.val();aFields.forEach(t=>{if(i[t]){var a="";if("timeStamp"==t){var l=parseInt(i.timeStamp);a=new Date(l).toUTCString().substring(0,22)}else a=i[t];n.getElementsByClassName(t)[0]&&(n.getElementsByClassName(t)[0].innerText=a)}"id"==t&&n.getElementsByClassName(t)[0]&&(n.getElementsByClassName(t)[0].innerText=e.key)});t.insertBefore(n,t.firstSibling);t.getElementsByClassName("clicker-"+e.key)[0].addEventListener("click",(function(){console.log("clicker:"+e.key)}))}(e,container)}));e.on("child_changed",(function(e){console.log("child_changed")}));e.on("child_removed",(function(e){container.getElementsByClassName("id-"+e.key)[0].parentElement.removeChild(datacontainer)}))}(firebase.database().ref(e+"/").orderByChild("name"));console.log("FBDataHandler:")}()}),!1);var container=d.getElementById("div-list");function writeData2FB(e="customer",t=aFields){var a=d.getElementById("id").value||fb.database().ref(e).push().key;d.getElementById("id").value=a;fb.database().ref(e).orderByKey().equalTo(a).once("value",n=>{n.exists()?console.log(e+" existing.",n.val()):console.log(e+" new.");var i=t.reduce((e,t)=>Object.assign({},e,"id"!==t?{[t]:d.getElementById(t).value}:{}),{});fb.database().ref(e+"/"+a).update(i)})}
