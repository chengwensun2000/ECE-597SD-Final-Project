import React, { useEffect, useState } from "react";
import { Button, StyleSheet, View } from "react-native";
import { ViroARScene, ViroARSceneNavigator, ViroText, ViroTrackingStateConstants } from "@viro-community/react-viro";
import firestore from "@react-native-firebase/firestore";
import database from '@react-native-firebase/database';
import { parseString } from "react-native-codegen/lib/parsers/flow";
let textstring = ''
let random = ''
function getData() {
  database()
    .ref('/Freqlog')
    .once('value')
    .then(snapshot => {
      random = snapshot.val().freq
      random = random.toString()
      console.log('User data: ', snapshot.val().freq);

    });
  return random +' hz'
}

function render(){
  return <ViroARSceneNavigator
    autofocus={true}
    initialScene={{
      scene: HelloWorldSceneAR,
    }}
    style={styles.f1}
  />
}
const HelloWorldSceneAR = () => {
  const [text, setText] = useState('Initializing AR...');
  let random = ''
  function onInitialized(state, reason) {
    console.log('guncellme', state, reason);
    let result = getData()

    if (state === ViroTrackingStateConstants.TRACKING_NORMAL) {

      console.log(result)
      setText(result)


      }
    else if (state === ViroTrackingStateConstants.TRACKING_NONE) {
      // Handle loss of tracking
    }


    }


  return (
    <ViroARScene onTrackingUpdated={onInitialized}>
      <ViroText
        text={text}
        scale={[0.5, 0.5, 0.5]}
        position={[0, 0, -1]}
        style={styles.helloWorldTextStyle}
      />
    </ViroARScene>
  );
};

export default () => {

  return (
    <View style = {{flex:1}}>
      <Button title={'Request new Frequency'} onPress={requestNewFreq}/>
      {render()}
    </View>
  );

};

var styles = StyleSheet.create({
  f1: {flex: 1},
  helloWorldTextStyle: {
    fontFamily: 'Arial',
    fontSize: 30,
    color: '#ffffff',
    textAlignVertical: 'center',
    textAlign: 'center',
  },
});

function requestNewFreq(){
  database()
    .ref('/Freqlog')
    .update({
      flag: true,
    })
    .then(() => console.log('Data set.'));
}
