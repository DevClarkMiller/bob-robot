import './App.css';
import { useNavigate, useSearchParams } from 'react-router-dom';

import { useEffect, useMemo } from 'react';
import ChatForm from './components/ChatForm';
import MessageViewer from './components/MessageViewer/MessageViewer';
import UnitGuidInput from './components/UnitGuidInput';
import useStoredField from './hooks/useStoredField';

function App() {
	const navigate = useNavigate();
	const [searchParams] = useSearchParams();

	const { value: unitGuid, setValue: setUnitGuid } = useStoredField('unitGuid');

	const paramToken = useMemo(() => searchParams.get('token'), [searchParams]);

	useEffect(() => {
		if (!paramToken) return;

		localStorage.setItem('token', paramToken);
		navigate({ search: '' }, { replace: true });
	}, [navigate, paramToken]);

	return (
		<div className="h-screen w-screen flex flex-col justify-center gap-2 text-center">
			<h1 className="font-bold text-lg mb-5">Bot the Robot Chatter</h1>
			<div className="w-100 mx-auto">
				<UnitGuidInput unitGuid={unitGuid} setUnitGuid={setUnitGuid} />
			</div>
			<div className="w-100 mx-auto">
				<MessageViewer unitGuid={unitGuid} />
			</div>
			<div className="w-100 mx-auto">
				<ChatForm />
			</div>
		</div>
	);
}

export default App;
