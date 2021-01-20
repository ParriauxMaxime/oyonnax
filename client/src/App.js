import React, { useLayoutEffect, useEffect, useState } from 'react'
import { Chart } from 'react-charts'
import { useData } from "./hooks/useData";

function App() {
  const {data, refresh} = useData();

  const chartData = React.useMemo(
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

  useEffect(() => {
    const interval = setInterval(() => {
      refresh();
    }, 5000)
    return () => {
      clearInterval(interval)
    };
  }, [])

  // need to refresh size, dunno why 
  const [workaround, setWorkaround] = useState(0)
  useLayoutEffect(() => {
    const interval = setInterval(() => setWorkaround(workaround === 1 ? 0 : 1), 500)
    setTimeout(() => clearInterval(interval) , 2000);
  // eslint-disable-next-line react-hooks/exhaustive-deps
  }, [])

  return (
    <div style={{ width: "100%", padding: 8 }}>
      <style dangerouslySetInnerHTML={{
        __html: `
        html,
        body {
          height: 100%;
          width: 100%;
          margin: 0;
        }

        #root {
          display: flex;
          justify-content: center;
          align-items: center;
          width: 100%;
          height: 100%;
        }
      `}}>
       </style>
      <div style={{ width: "100%", height: `calc(65vh + ${workaround}px)` }}>
        <Chart data={chartData} axes={axes} ></Chart>
      </div>
    </div>
  );
}

export default App;
