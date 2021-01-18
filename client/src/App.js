import React, { useLayoutEffect, useState } from 'react'
import { Chart } from 'react-charts'
import { useData } from "./hooks/useData";

function App() {
  const {data, postData} = useData();

  const fakeData = React.useMemo(
    () => [
      {
        label: 'Temperature',
        data: data.map(({temperature, timestamp}) => [new Date(timestamp), temperature])
      },
    ],
    [data]
  )
 
  const axes = React.useMemo(
    () => [
      { primary: true, type: 'time', position: 'bottom' },
      { type: 'linear', position: 'left' }
    ],
    []
  )

  // need to refresh size, dunno why 
  const [workaround, setWorkaround] = useState(0)
  useLayoutEffect(() => {
    const interval = setInterval(() => setWorkaround(workaround === 1 ? 0 : 1), 500)
    setTimeout(() => clearInterval(interval), 2000);
  }, [workaround])

  return (
    <div style={{ height: "100%", padding: 8 }}>
      <style dangerouslySetInnerHTML={{
        __html: `
        html,
        body {
          height: 100%;
          width: 100%;
          margin: 0;
        }
      `}}>
       </style>
      <div style={{ width: "100%", height: 600 + workaround, marginBottom: 24 }}>
        <Chart data={fakeData} axes={axes} ></Chart>
      </div>
      {/* <button onClick={() => { postData(20 + ((Math.random() % 2 === 0 ? -1 : 1) * Math.random() * 7) ) }}>Add value</button> */}
    </div>
  );
}

export default App;
