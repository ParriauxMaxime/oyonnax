import { useCallback, useState } from "react";


const hostname = window.INTERNAL_HOSTNAME || 'localhost:3000';
const endpoint = `http://${hostname}/data`

export const useData = () => {
  const [data, setData] = useState([]);
  const [loading, setLoading] = useState(false);
  const [refresh, setRefresh] = useState(true);

  const postData = useCallback((value) => {
    fetch(endpoint, {
      method: 'POST',
      headers: {
      'Content-type': 'application/json'
      },
      body: JSON.stringify({
        timestamp: Date.now(),
        temperature: value
      })
    }).then(() => {
      setRefresh(true);
    })
  }, [])

  const fetchData = useCallback(() => {
    setRefresh(false)
    setLoading(true);
    fetch(endpoint).then(res => res.json()).then((res) => {
      setData(res)
      setLoading(false);
    })
  // eslint-disable-next-line react-hooks/exhaustive-deps
  }, [refresh, loading])

  const refreshFunction = useCallback(() => {
    setRefresh(true);
  }, [])

  if (!loading && refresh) {
    fetchData();
  }

  return {data, postData, refresh: refreshFunction};
}