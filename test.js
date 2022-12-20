import firestore from '@react-native-firebase/firestore';
function setFrequency(){
  firestore()
    .collection('Users')
    .doc('ABC')
    .set({
      name: 'Ada Lovelace',
      age: 30,
    })
    .then(() => {
      console.log('User addd!');
    });
}
setFrequency()
