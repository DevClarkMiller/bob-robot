import './App.css';
import { useNavigate, useSearchParams } from 'react-router-dom';

import { useEffect, useMemo } from 'react';
import ChatForm from './components/ChatForm';
import MessageViewer from './components/MessageViewer/MessageViewer';
import SettingsInput from './components/SettingsInput';
import useSettings from './hooks/useSettings';

function App() {
	const navigate = useNavigate();
	const [searchParams] = useSearchParams();

	const { isLoading, isUsingAIChats, unitGuid, setUnitGuid, onChangeIsUsingAIChats } = useSettings();
	const paramToken = useMemo(() => searchParams.get('token'), [searchParams]);

	useEffect(() => {
		if (!paramToken) return;

		localStorage.setItem('token', paramToken);
		navigate({ search: '' }, { replace: true });
	}, [navigate, paramToken]);

	// TODO: ADD TOGGLE TO MAKE CHATGPT RESPOND TO CHATS INSTEAD WHEN I DONT FEEL LIKE IT
	// so add a new controller, add route to toggle chat gpt, when chats come in, check if chatgpt
	// is toggled, if so respond right away, else add to queue

	return (
		<div className="h-screen w-screen flex flex-col justify-center gap-2 text-center">
			<h1 className="font-bold text-lg mb-5">Bot the Robot Chatter</h1>
			<div className="w-100 mx-auto">
				<SettingsInput
					isLoading={isLoading}
					isUsingAIChats={isUsingAIChats}
					unitGuid={unitGuid}
					setUnitGuid={setUnitGuid}
					onChangeIsUsingAIChats={onChangeIsUsingAIChats}
				/>
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
